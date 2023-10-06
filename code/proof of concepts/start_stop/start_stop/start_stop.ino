const int switchPin = 2;
const int ledPin = 13;
volatile bool isRunning = false;

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(switchPin), toggleState, RISING);
}

void loop() {
  if (isRunning) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void toggleState() {
  isRunning = !isRunning;
}


