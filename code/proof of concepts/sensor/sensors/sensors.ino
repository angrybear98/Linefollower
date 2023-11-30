const int sensorPins[] = {A0, A1, A2, A3, A4, A5}; // Analog pins for sensors
const int numSensors = 6;

const int motorPin1 = 3;  // Right wheel
const int motorPin2 = 5;  // Left wheel

const int threshold = 500;  // Adjust this value based on sensor readings
const int baseSpeed = 150;  // Adjust this value based on desired speed

void setup() {
  for (int i = 0; i < numSensors; ++i) {
    pinMode(sensorPins[i], INPUT);
  }
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
}

void loop() {
  int sensorValues[numSensors];

  // Read sensor values
  for (int i = 0; i < numSensors; ++i) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Calculate error (difference between left and right sensors)
  int error = sensorValues[3] - sensorValues[2];

  // Adjust motor speeds based on the error
  int rightSpeed = baseSpeed + error;
  int leftSpeed = baseSpeed - error;

  // Make sure motor speeds are within a valid range
  rightSpeed = constrain(rightSpeed, 0, 255);
  leftSpeed = constrain(leftSpeed, 0, 255);

  // Control the motors using the H-bridge
  analogWrite(motorPin1, rightSpeed);
  analogWrite(motorPin2, leftSpeed);
}
