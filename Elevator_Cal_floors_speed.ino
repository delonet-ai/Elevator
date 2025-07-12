// Elevator Sketch with Floor Call Logic
// Calibration (upper and lower limits) then floor calls for 3 floors

#include <Arduino.h>

// Pin Definitions
#define BTN_UP        10    // Manual UP (INPUT_PULLUP)
#define BTN_DOWN      11    // Manual DOWN
#define BTN_FLOOR1    7     // Floor1: first sets lower limit, then floor call
#define BTN_FLOOR2    8     // Floor2 call
#define BTN_FLOOR3    9     // Floor3 call
#define PIN_STEP      13    // Stepper STEP pin
#define PIN_DIR       A3    // Stepper DIR pin
#define LIMIT_TOP     A1    // Top limit switch (INPUT_PULLUP)
#define PIN_POT       A2    // Potentiometer for speed factor (analog input)

// LED Pins (active-low: LOW = ON)
#define LED_UP        5
#define LED_DOWN      6
#define LED_FLOOR1    2
#define LED_FLOOR2    3
#define LED_FLOOR3    4

// Timing constants
const unsigned int STEP_DELAY       = 100;  // base delay in microseconds
const unsigned long BLINK_INTERVAL  = 200;   // ms for floor LEDs fast blink

// State tracking
long positionCounter    = 0;
bool upperLimitLocked   = false;
bool lowerLimitSet      = false;
long lowerLimit         = 0;       // threshold for DOWN movement
long floor3Limit        = 0;       // 2% from lowerLimit

// Floor call logic
enum MoveMode { MANUAL, AUTO };
MoveMode mode = MANUAL;
long targetPosition = -1;

// Edge tracking
bool lastFloor1State    = HIGH;
bool lastFloor2State    = HIGH;
bool lastFloor3State    = HIGH;
bool lastLimitState     = HIGH;

// LED blink for floor buttons
unsigned long blinkLastTime = 0;
bool blinkState            = false;

// Speed factor variable
unsigned int stepDelay = STEP_DELAY;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("[Setup] Elevator ready");

  // Configure inputs
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_FLOOR1, INPUT_PULLUP);
  pinMode(BTN_FLOOR2, INPUT_PULLUP);
  pinMode(BTN_FLOOR3, INPUT_PULLUP);
  pinMode(LIMIT_TOP, INPUT_PULLUP);
  pinMode(PIN_POT, INPUT);

  // Configure motor outputs
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);

  // Configure LEDs
  pinMode(LED_UP, OUTPUT); digitalWrite(LED_UP, HIGH);
  pinMode(LED_DOWN, OUTPUT); digitalWrite(LED_DOWN, HIGH);
  pinMode(LED_FLOOR1, OUTPUT); digitalWrite(LED_FLOOR1, HIGH);
  pinMode(LED_FLOOR2, OUTPUT); digitalWrite(LED_FLOOR2, HIGH);
  pinMode(LED_FLOOR3, OUTPUT); digitalWrite(LED_FLOOR3, HIGH);
}

void loop() {
  unsigned long now = millis();

  // Read speed factor from potentiometer
  int potVal = analogRead(PIN_POT);             // 0..1023
  unsigned int factor = map(potVal, 0, 1023, 1, 15);
  stepDelay = STEP_DELAY * factor;

  // Read inputs
  bool upHeld     = (digitalRead(BTN_UP) == LOW);
  bool downHeld   = (digitalRead(BTN_DOWN) == LOW);
  bool f1         = (digitalRead(BTN_FLOOR1) == LOW);
  bool f2         = (digitalRead(BTN_FLOOR2) == LOW);
  bool f3         = (digitalRead(BTN_FLOOR3) == LOW);
  bool limitTop   = (digitalRead(LIMIT_TOP) == LOW);

  // Handle top limit (edge)
  if (limitTop && lastLimitState == HIGH) {
    positionCounter = 0;
    upperLimitLocked = true;
    Serial.println("[Calib] Top set, Counter=0");
  }
  lastLimitState = limitTop;

  // Lower limit setup: only if upper calibrated but lower not set
  if (upperLimitLocked && !lowerLimitSet && f1 && lastFloor1State == HIGH) {
    lowerLimitSet = true;
    lowerLimit = positionCounter;
    floor3Limit = (long)(lowerLimit * 0.02);  // compute third floor offset
    Serial.print("[Calib] Lower set="); Serial.println(lowerLimit);
    Serial.print("[Calib] Floor3 limit="); Serial.println(floor3Limit);
  }

  // Floor calls: only after both limits set, f1 now acts as call
  if (upperLimitLocked && lowerLimitSet && mode == MANUAL) {
    if (f1 && lastFloor1State == HIGH) {
      targetPosition = lowerLimit;
      mode = AUTO;
      Serial.print("[Call] Floor1 target="); Serial.println(targetPosition);
    } else if (f2 && lastFloor2State == HIGH) {
      targetPosition = lowerLimit / 2;
      mode = AUTO;
      Serial.print("[Call] Floor2 target="); Serial.println(targetPosition);
    } else if (f3 && lastFloor3State == HIGH) {
      targetPosition = (long)(lowerLimit * 0.02);
      mode = AUTO;
      Serial.print("[Call] Floor3 target="); Serial.println(targetPosition);
    }
  }
  lastFloor1State = f1;
  lastFloor2State = f2;
  lastFloor3State = f3;

  // Update blink state for floor LEDs
  if (now - blinkLastTime >= BLINK_INTERVAL) {
    blinkLastTime = now;
    blinkState = !blinkState;
  }

  // Floor LED logic
  if (!upperLimitLocked) {
    // Before top limit set: blink floors 1-3
    digitalWrite(LED_FLOOR1, blinkState ? LOW : HIGH);
    digitalWrite(LED_FLOOR2, blinkState ? LOW : HIGH);
    digitalWrite(LED_FLOOR3, blinkState ? LOW : HIGH);
  } else if (upperLimitLocked && !lowerLimitSet) {
    // After top limit only: Floor1 steady on
    digitalWrite(LED_FLOOR1, LOW);
    digitalWrite(LED_FLOOR2, HIGH);
    digitalWrite(LED_FLOOR3, HIGH);
  } else {
    // After both limits: default floors off
    digitalWrite(LED_FLOOR1, HIGH);
    digitalWrite(LED_FLOOR2, HIGH);
    digitalWrite(LED_FLOOR3, HIGH);
  }

  // Floor call indication during AUTO mode
  if (mode == AUTO) {
    // Light target floor LED until arrival
    if (targetPosition == lowerLimit) {
      digitalWrite(LED_FLOOR1, LOW);
    } else if (targetPosition == lowerLimit/2) {
      digitalWrite(LED_FLOOR2, LOW);
    } else if (targetPosition == (long)(lowerLimit*0.02)) {
      digitalWrite(LED_FLOOR3, LOW);
    }
  }

  // UP/DOWN LED logic: on when movement allowed (manual or auto)
  bool canUp;
  if (mode == MANUAL && lowerLimitSet) canUp = (positionCounter > floor3Limit);
  else canUp = (!upperLimitLocked || positionCounter > 0);
  bool canDown = (!lowerLimitSet || positionCounter < lowerLimit);
  digitalWrite(LED_UP,   canUp ? LOW : HIGH);
  digitalWrite(LED_DOWN, canDown ? LOW : HIGH);

  // Movement logic
  if (mode == MANUAL) {
    // Manual UP limited by floor3Limit after calibration
    if (upHeld && ((lowerLimitSet && positionCounter > floor3Limit) || (!lowerLimitSet && (!upperLimitLocked || positionCounter > 0)))) stepMove(1);
    if (downHeld && (!lowerLimitSet || positionCounter < lowerLimit)) stepMove(-1);
  } else {
    if (positionCounter < targetPosition) stepMove(-1);
    else if (positionCounter > targetPosition) stepMove(1);
    else { Serial.println("[Arrive] Target reached"); mode = MANUAL; }
  }
}

// stepMove: dir +1 up, -1 down
void stepMove(int dir) {
  digitalWrite(PIN_DIR, dir>0?HIGH:LOW);
  digitalWrite(PIN_STEP, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(PIN_STEP, LOW);
  delayMicroseconds(stepDelay);
  positionCounter += (dir<0?1:-1);
}
