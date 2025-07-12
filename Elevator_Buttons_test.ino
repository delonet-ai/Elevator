// Button Test Sketch - Prints button and switch states to Serial

// Pin Definitions
#define BTN_FLOOR1    7   // 1st floor button
#define BTN_FLOOR2    8   // 2nd floor button
#define BTN_FLOOR3    9   // 3rd floor button
#define BTN_UP        10  // Up manual
#define BTN_DOWN      11  // Down manual
#define BTN_RESET     A0  // Stop/Reset
#define LIMIT_TOP     A1  // Limit switch (Top)

// State tracking for edge detection
bool lastStateFloor1 = HIGH;
bool lastStateFloor2 = HIGH;
bool lastStateFloor3 = HIGH;
bool lastStateUp     = HIGH;
bool lastStateDown   = HIGH;
bool lastStateReset  = HIGH;
bool lastStateLimit  = HIGH;

void setup() {
  Serial.begin(9600);
  Serial.println("[Setup] Button test started");

  // Configure pins
  pinMode(BTN_FLOOR1, INPUT_PULLUP);
  pinMode(BTN_FLOOR2, INPUT_PULLUP);
  pinMode(BTN_FLOOR3, INPUT_PULLUP);
  pinMode(BTN_UP,     INPUT_PULLUP);
  pinMode(BTN_DOWN,   INPUT_PULLUP);
  pinMode(BTN_RESET,  INPUT_PULLUP);
  pinMode(LIMIT_TOP,  INPUT_PULLUP);
}

void loop() {
  // Read current states
  bool cur1 = digitalRead(BTN_FLOOR1);
  bool cur2 = digitalRead(BTN_FLOOR2);
  bool cur3 = digitalRead(BTN_FLOOR3);
  bool curUp   = digitalRead(BTN_UP);
  bool curDown = digitalRead(BTN_DOWN);
  bool curReset= digitalRead(BTN_RESET);
  bool curLimit= digitalRead(LIMIT_TOP);

  // Edge detection: press
  if (cur1 == LOW && lastStateFloor1 == HIGH)    Serial.println("[Press] Floor 1 button");
  if (cur2 == LOW && lastStateFloor2 == HIGH)    Serial.println("[Press] Floor 2 button");
  if (cur3 == LOW && lastStateFloor3 == HIGH)    Serial.println("[Press] Floor 3 button");
  if (curUp == LOW && lastStateUp == HIGH)       Serial.println("[Press] UP button");
  if (curDown == LOW && lastStateDown == HIGH)   Serial.println("[Press] DOWN button");
  if (curReset == LOW && lastStateReset == HIGH) Serial.println("[Press] RESET button");
  if (curLimit == LOW && lastStateLimit == HIGH) Serial.println("[Trigger] LIMIT_TOP switch");

  // Edge detection: release
  if (cur1 == HIGH && lastStateFloor1 == LOW)    Serial.println("[Release] Floor 1 button");
  if (cur2 == HIGH && lastStateFloor2 == LOW)    Serial.println("[Release] Floor 2 button");
  if (cur3 == HIGH && lastStateFloor3 == LOW)    Serial.println("[Release] Floor 3 button");
  if (curUp == HIGH && lastStateUp == LOW)       Serial.println("[Release] UP button");
  if (curDown == HIGH && lastStateDown == LOW)   Serial.println("[Release] DOWN button");
  if (curReset == HIGH && lastStateReset == LOW) Serial.println("[Release] RESET button");
  if (curLimit == HIGH && lastStateLimit == LOW) Serial.println("[Reset] LIMIT_TOP switch released");

  // Update last states
  lastStateFloor1 = cur1;
  lastStateFloor2 = cur2;
  lastStateFloor3 = cur3;
  lastStateUp     = curUp;
  lastStateDown   = curDown;
  lastStateReset  = curReset;
  lastStateLimit  = curLimit;

  delay(50); // debounce delay
}
