#include "SerialCommand.h"

#define SerialPort Serial
#define BluetoothPort Serial1
#define Baudrate 9600

// Define the pin numbers for the analog sensors
const int sensorPins[6] = {A0, A1, A2, A3, A4, A5};  // Analog pins where the QTR sensors are connected

// Variables to store sensor values
int sensorValues[6];

int sensorMin[6] = {296, 114, 59, 55, 62, 268};  // White area
int sensorMax[6] = {815, 764, 735, 740, 757, 831};  // Black area

// Threshold for detecting a line (adjust based on your setup)
const int threshold = 400;  // This value might need tuning based on the surface and sensor

// Time management
unsigned long previousMillis = 0;    // Stores the last time the sensor readings were sent
const long interval = 1000;          // Interval between sensor readings (in milliseconds)

void setup() {
  // Initialize Serial communication for debugging purposes
  Serial.begin(9600);

  // Initialize Serial1 communication for Bluetooth module
  Serial1.begin(9600);
  
  // Set analog pins as input (optional since they are by default)
  for (int i = 0; i < 6; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  // Get the current time
  unsigned long currentMillis = millis();

  // Check if the time interval has passed
  if (currentMillis - previousMillis >= interval) {
    // Save the last time the sensor values were sent
    previousMillis = currentMillis;

    // Read values from the sensors and detect lines
    for (int i = 0; i < 6; i++) {
      sensorValues[i] = analogRead(sensorPins[i]);
    }
    
    // Print the sensor values to Serial1 (Bluetooth module)
    Serial1.print("Line Detection: ");
    for (int i = 0; i < 6; i++) {
      Serial1.print(sensorValues[i]);
      if (i < 5) {
        Serial1.print(", "); // Add comma between values
      }
    }
    Serial1.println(); // New line after sending all values
    
    // Print the sensor values to Serial
    Serial.print("Line Detection: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(sensorValues[i]);
      if (i < 5) {
        Serial.print(", "); // Add comma between values
      }
    }
    Serial.println(); // New line after printing all values
  }

  // Other tasks can be performed here while waiting for the next interval
}







