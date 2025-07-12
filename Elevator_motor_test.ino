// Continuous Motor Control Sketch
// Motor runs continuously while UP or DOWN button is held

// Pin Definitions
#define BTN_UP     10   // UP button (INPUT_PULLUP)
#define BTN_DOWN   11   // DOWN button (INPUT_PULLUP)
#define PIN_STEP   13   // Stepper STEP pin
#define PIN_DIR    A3   // Stepper DIR pin

// Step timing (microseconds)
const unsigned int STEP_DELAY = 1000;  // adjust for desired speed

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("[Setup] Continuous motor control ready");

  // Configure pins
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
}

void loop() {
  bool upHeld = (digitalRead(BTN_UP) == LOW);
  bool downHeld = (digitalRead(BTN_DOWN) == LOW);

  if (upHeld) {
    // Rotate up continuously
    digitalWrite(PIN_DIR, HIGH);
    stepOnce();
  } else if (downHeld) {
    // Rotate down continuously
    digitalWrite(PIN_DIR, LOW);
    stepOnce();
  }
  // If neither is held, motor stops (no further pulses)
}

// Perform a single step pulse
void stepOnce() {
  digitalWrite(PIN_STEP, HIGH);
  delayMicroseconds(STEP_DELAY);
  digitalWrite(PIN_STEP, LOW);
  delayMicroseconds(STEP_DELAY);
}
