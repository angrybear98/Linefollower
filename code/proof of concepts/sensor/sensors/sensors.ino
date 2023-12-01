const int sensorPins[] = {A0, A1, A2, A3, A4, A5}; // Analog pins for sensors
const int numSensors = 6;

const int motorPin1 = 3;  // Right wheel
const int motorPin2 = 5;  // Left wheel

const int threshold = 80;   // Threshold for considering a black line
const int baseSpeed = 150;  // Adjust this value based on desired speed
const float Kp = 0.5;       // Proportional gain

unsigned long lastSensorPrintTime = 0;
const unsigned long sensorPrintInterval = 1000; // Print every 1000 milliseconds (1 second)

void setup() {
  Serial.begin(9600); // Initialize serial communication
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

  // Check if all sensors are below the threshold (indicating no black line)
  if (areAllSensorsBelowThreshold(sensorValues, threshold)) {
    // Stop the motors
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);

    // Print a message indicating no black line detected
    Serial.println("No black line detected. Motors stopped.");
  } else {
    // Calculate error (difference between left and right sensors)
    int error = sensorValues[3] - sensorValues[2];

    // Adjust motor speeds based on the error using proportional control
    int rightSpeed = baseSpeed + int(Kp * error);
    int leftSpeed = baseSpeed - int(Kp * error);

    // Make sure motor speeds are within a valid range
    rightSpeed = constrain(rightSpeed, 0, 255);
    leftSpeed = constrain(leftSpeed, 0, 255);

    // Control the motors using the H-bridge
    analogWrite(motorPin1, rightSpeed);
    analogWrite(motorPin2, leftSpeed);
  }

  // Check if it's time to print sensor values
  unsigned long currentTime = millis();
  if (currentTime - lastSensorPrintTime >= sensorPrintInterval) {
    // Print sensor values to Serial Monitor
    Serial.println("Sensor Values:");
    for (int i = 0; i < numSensors; ++i) {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(sensorValues[i]);
    }

    // Update the last sensor print time
    lastSensorPrintTime = currentTime;
  }
}

bool areAllSensorsBelowThreshold(const int values[], int threshold) {
  // Check if all sensor values are below the threshold
  for (int i = 0; i < numSensors; ++i) {
    if (values[i] >= threshold) {
      return false; // At least one sensor is above or equal to the threshold
    }
  }
  return true; // All sensors are below the threshold
}
