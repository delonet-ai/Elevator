// Continuous Motor Control with Counter, Limit Switch, and LED Indication

#include <Arduino.h>

// Pin Definitions
#define BTN_UP        10    // UP button (INPUT_PULLUP)
#define BTN_DOWN      11    // DOWN button (INPUT_PULLUP)
#define BTN_FLOOR1    7     // Floor1 limit set button (INPUT_PULLUP)
#define PIN_STEP      13    // Stepper STEP pin
#define PIN_DIR       A3    // Stepper DIR pin
#define LIMIT_TOP     A1    // Top limit switch (INPUT_PULLUP)

// LED Pins (active-low: LOW = ON)
#define LED_UP        5     // UP LED
#define LED_DOWN      6     // DOWN LED
#define LED_FLOOR1    2     // Floor1 LED (blink fast)
#define LED_FLOOR2    3     // Floor2 LED
#define LED_FLOOR3    4     // Floor3 LED

// Timing constants
const unsigned int STEP_DELAY       = 1000;  // us between motor steps
const unsigned long BLINK_INTERVAL  = 200;   // ms for floor LEDs fast blink

// State tracking
long positionCounter    = 0;
bool upperLimitLocked   = false;   // locks UP after top limit
bool lowerLimitSet      = false;
long lowerLimit         = 0;       // threshold for DOWN movement

// Edge tracking
bool lastFloor1State    = HIGH;
bool lastLimitState     = HIGH;

// LED blink state for floor LEDs
unsigned long blinkLastTime = 0;
bool blinkState            = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("[Setup] Motor + LEDs ready");

  // Configure inputs
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_FLOOR1, INPUT_PULLUP);
  pinMode(LIMIT_TOP, INPUT_PULLUP);

  // Configure motor outputs
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);

  // Configure LED outputs (active-low)
  pinMode(LED_UP, OUTPUT);
  pinMode(LED_DOWN, OUTPUT);
  pinMode(LED_FLOOR1, OUTPUT);
  pinMode(LED_FLOOR2, OUTPUT);
  pinMode(LED_FLOOR3, OUTPUT);

  // Turn off LEDs initially
  digitalWrite(LED_UP, HIGH);
  digitalWrite(LED_DOWN, HIGH);
  digitalWrite(LED_FLOOR1, HIGH);
  digitalWrite(LED_FLOOR2, HIGH);
  digitalWrite(LED_FLOOR3, HIGH);
}

void loop() {
  unsigned long now = millis();

  // Read inputs
  bool upHeld     = (digitalRead(BTN_UP) == LOW);
  bool downHeld   = (digitalRead(BTN_DOWN) == LOW);
  bool floor1     = (digitalRead(BTN_FLOOR1) == LOW);
  bool topSwitch  = (digitalRead(LIMIT_TOP) == LOW);

  // Handle top limit (edge)
  if (topSwitch && lastLimitState == HIGH) {
    Serial.println("[Limit] Top switch activated");
    positionCounter = 0;
    upperLimitLocked = true;
    Serial.println("[Counter]=0, UP locked");
  }
  lastLimitState = topSwitch;

  // Handle lower limit set by Floor1 (edge)
  if (floor1 && lastFloor1State == HIGH) {
    lowerLimitSet = true;
    lowerLimit = positionCounter;
    Serial.print("[LimitLow] set="); Serial.println(lowerLimit);
  }
  lastFloor1State = floor1;

  // Update blink state for floor LEDs
  if (now - blinkLastTime >= BLINK_INTERVAL) {
    blinkLastTime = now;
    blinkState = !blinkState;
  }

  // Floor LED logic: blink fast until upper limit set
  if (!upperLimitLocked) {
    digitalWrite(LED_FLOOR1, blinkState ? LOW : HIGH);
    digitalWrite(LED_FLOOR2, blinkState ? LOW : HIGH);
    digitalWrite(LED_FLOOR3, blinkState ? LOW : HIGH);
  } else {
    // After upper limit: Floor1 on until lower limit set, others off
    digitalWrite(LED_FLOOR1, lowerLimitSet ? HIGH : LOW);
    digitalWrite(LED_FLOOR2, HIGH);
    digitalWrite(LED_FLOOR3, HIGH);
  }

  // UP/DOWN LED logic: glow when active (available to move)
  // UP active if UP command allowed
  if (!upperLimitLocked || positionCounter > 0) {
    digitalWrite(LED_UP, LOW);
  } else {
    digitalWrite(LED_UP, HIGH);
  }

  // DOWN active if DOWN command allowed
  if (!lowerLimitSet || positionCounter < lowerLimit) {
    digitalWrite(LED_DOWN, LOW);
  } else {
    digitalWrite(LED_DOWN, HIGH);
  }

  // Motor control
  if (upHeld && (!upperLimitLocked || positionCounter > 0)) {
    digitalWrite(PIN_DIR, HIGH);
    stepOnce(); positionCounter--; }
  if (downHeld && (!lowerLimitSet || positionCounter < lowerLimit)) {
    digitalWrite(PIN_DIR, LOW);
    stepOnce(); positionCounter++; }
}

void stepOnce() {
  digitalWrite(PIN_STEP, HIGH);
  delayMicroseconds(STEP_DELAY);
  digitalWrite(PIN_STEP, LOW);
  delayMicroseconds(STEP_DELAY);
}
