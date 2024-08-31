#include "SerialCommand.h"
#include "EEPROMAnything.h"
#include "QTRSensors.h"

#define SerialPort Serial
#define BluetoothPort Serial1
#define Baudrate 9600

// Define motor pins
#define ENA 9   // Enable pin for Motor A
#define IN1 4   // Input 1 for Motor A
#define IN2 5   // Input 2 for Motor A
#define ENB 10   // Enable pin for Motor B
#define IN3 7   // Input 1 for Motor B
#define IN4 8   // Input 2 for Motor B

const int sensor[6] = {A0, A1, A2, A3, A4, A5};


unsigned long searchStartTime = 0;
bool isSearching = false;

const int switchPin = 2;
const int ledPin = LED_BUILTIN;
volatile bool isRunning;

//SerialCommand sCmd(SerialPort);
SerialCommand sCmd(BluetoothPort);


//SoftwareSerial BluetoothSerial(BluetoothRx, BluetoothTx);

bool debug;
unsigned long previous, calculationTime, lastToggleTime;

struct param_t
{
  unsigned long cycleTime;
  unsigned long blinkPeriod; // New parameter for LED blinking
  float kp;
  float ki;
  float kd;
  int max;
  int threshold;
  int base;
  int black[6];
  int white[6];
  /* other parameters that need to be stored in EEPROM go here ... */
} params;

  double iTerm;

// EEPROM addresses for storing PID values

// PID variables
float error = 0, previous_error = 0;
float correction = 0;

void setup()
{
  attachInterrupt(digitalPinToInterrupt(0), onStop, CHANGE);
  isRunning = false;
  SerialPort.begin(Baudrate);
  BluetoothPort.begin(Baudrate); // Change the baud rate if needed

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("start", onStart);
  sCmd.addCommand("stop", onStop);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  // Set motor pins as output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Set sensor pins as input
for (int i = 0; i < 6; i++)
{
  pinMode(sensor[i], INPUT);
}   

  // Read PID values from EEPROM
  EEPROM_readAnything(0, params);

  // If no values are found, use default PID values
  if (isnan(params.kp) || isnan(params.ki) || isnan(params.kd)) {
    EEPROM_writeAnything(0, params);
  }
}

void loop()
{
    sCmd.readSerial();
 
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;
      unsigned long current = micros();
      if (current - previous >= params.cycleTime)
  {
    previous = current;
  }
    // Your cyclic code goes here...
    if (isRunning == true)
    {
      followLine();
    }
    // Normalize and interpolate sensor

    // PID control

    // Drive motors

  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}


void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
  BluetoothPort.print("unknown command: \"");
  BluetoothPort.print(command);
  BluetoothPort.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  SerialPort.print("Received set command. Param: ");
  SerialPort.print(param);
  SerialPort.print(", Value: ");
  SerialPort.println(value);

  if (strcmp(param, "cycle") == 0)
  {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio;

    params.cycleTime = newCycleTime;
  }
  else if (strcmp(param, "blink") == 0) {
    params.blinkPeriod = atol(value);
    SerialPort.println("Setting blinkPeriod to: " + String(params.blinkPeriod));
    BluetoothPort.println("Setting blinkPeriod to: " + String(params.blinkPeriod));
  }
  else if (strcmp(param, "kp") == 0) {
    params.kp = atol(value);
    SerialPort.println("Setting P-value to: " + String(params.kp));
    BluetoothPort.println("Setting P-value to: " + String(params.kp));
  }
  else if (strcmp(param, "ki") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * cycleTimeInSec;
    SerialPort.println("Setting I-value to: " + String(params.ki));
    BluetoothPort.println("Setting I-value to: " + String(params.ki));    
  }
  else if (strcmp(param, "kd") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.kd = atof(value) / cycleTimeInSec;
    SerialPort.println("Setting D-value to: " + String(params.kd));
    BluetoothPort.println("Setting D-value to: " + String(params.kd));       
  }
  else if (strcmp(param, "max") == 0) {
    params.max = atol(value);
    SerialPort.println("Setting max speed to: " + String(params.max));
    BluetoothPort.println("Setting max speed to: " + String(params.max));
  }
  else if (strcmp(param, "base") == 0) {
    params.base = atol(value);
    SerialPort.println("Setting base speed to: " + String(params.base));
    BluetoothPort.println("Setting base speed to: " + String(params.base));    
  }  
  else if (strcmp(param, "threshold") == 0) {
    params.threshold = atol(value);
    SerialPort.println("Setting sensor threshold to: " + String(params.threshold));
    BluetoothPort.println("Setting sensor threshold to: " + String(params.threshold));
  }
     
  /* Add other commands for setting parameters here ... */
  
  EEPROM_writeAnything(0, params);
}

void onStart()
{
  SerialPort.println("starting");
  BluetoothPort.println("starting");
    isRunning = true;
    iTerm = 0;
}

void onStop()
{
  SerialPort.println("stopping");
  BluetoothPort.println("stopping");
  isRunning = false;
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);  
}

void onDebug()
{
    char* param = sCmd.next();

    if (strcmp(param, "calibration") == 0) {
    SerialPort.println("black calibration: ");
    BluetoothPort.println("black calibration: ");
    for (int i = 0; i < 6; i++)
    {
      SerialPort.print(params.black[i]);
      SerialPort.print(" ");
      BluetoothPort.print(params.black[i]);
      BluetoothPort.print(" ");      
    }
      SerialPort.println(" ");
      BluetoothPort.println(" ");      

    SerialPort.println("white calibration: ");
    BluetoothPort.print("white calibration: ");    
    for (int i = 0; i < 6; i++)
    {
      SerialPort.print(params.white[i]);
      SerialPort.print(" ");
      BluetoothPort.print(params.white[i]);
      BluetoothPort.print(" ");      
    }
      SerialPort.println(" ");
      BluetoothPort.println(" ");          
      }
      else{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  BluetoothPort.println("cycle time: " + String(params.cycleTime));

  SerialPort.print("blink period: ");
  SerialPort.println(params.blinkPeriod);
  BluetoothPort.println("blink period: " + String(params.blinkPeriod));
  
  SerialPort.print("P-value: ");
  SerialPort.println(params.kp);
  BluetoothPort.println("P-value: " + String(params.kp));

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  BluetoothPort.print("I-value: ");
  BluetoothPort.println(ki);

  float kd = params.kd * cycleTimeInSec;
  BluetoothPort.print("D-value: ");
  BluetoothPort.println(kd);

  SerialPort.print("maxSpeed: ");
  SerialPort.println(params.max);
  BluetoothPort.println("maxSpeed: " + String(params.max));

  SerialPort.print("baseSpeed: ");
  SerialPort.println(params.base);
  BluetoothPort.println("baseSpeed: " + String(params.base));               

  SerialPort.print("sensor threshold: ");
  SerialPort.println(params.threshold);
  BluetoothPort.println("threshold: " + String(params.threshold));   
  /* Add code to display other debug information here... */
  
  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  BluetoothPort.println("calculation time: " + String(calculationTime));
  calculationTime = 0;
      }

}

// Function to set motor speeds
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  //BluetoothPort.println(error);
  int error = readSensors();
  if(error < 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    //leftSpeed = -leftSpeed;  // Invert speed for reverse direction
  }
  
  if(error > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    //rightSpeed = -rightSpeed;  // Invert speed for reverse direction
  }
  
  analogWrite(ENA, abs(constrain(leftSpeed, 35, params.max)));
  analogWrite(ENB, abs(constrain(rightSpeed, 35, params.max)));
  //BluetoothPort.println(leftSpeed);
  //BluetoothPort.print(" leftSpeed");
  //BluetoothPort.println(rightSpeed);
  //BluetoothPort.print(" rightSpeed");
  //BluetoothPort.println();


}

// Function to read sensors and calculate error
int readSensors() {
  int normalised[6];
  int reformed[6];
  int sensorvalues[6];
  for (int i = 0; i < 6; i++)
  {
    sensorvalues[i] = constrain(analogRead(sensor[i]), 0, 1000);
  }

  for (int i = 0; i < 6; i++)
  {
    reformed[i] = map(sensorvalues[i], params.white[i], params.black[i], 0, 1000);
  }

  for (int i = 0; i < 6; i++)
  {
    normalised[i] = constrain((reformed[i]), 0, 1000);
  }    
   
    int schaal = (normalised[0]*(-5) + normalised[1]*(-3) + normalised[2]*(-1) + normalised[3]*(1) + normalised[4]*(3) + normalised[5]*(5));
    int som = (normalised[0]+normalised[1]+normalised[2]+normalised[3]+normalised[4]+normalised[5]);
    int error = schaal/som;
    //BluetoothPort.println(error);

    return error;

}

/*float followLineAndMeasureError() {
  // Simulate line-following for a short distance and return the error
  float error = 0;

  for (int t = 0; t < 100; t++) {
    error += readSensors();  // Replace with actual sensor reading function
    delay(10);  // Simulate time for following the line
  }

  return error / 100;  // Return the average error
}*/

void followLine() {
    // Read sensors and calculate error
    int error = readSensors();

  

    // Detect if no line is detected
  if   (analogRead(sensor[0]) < params.threshold && analogRead(sensor[1]) < params.threshold &&
        analogRead(sensor[2]) < params.threshold && analogRead(sensor[3]) < params.threshold &&
        analogRead(sensor[4]) < params.threshold && analogRead(sensor[5]) < params.threshold) {
        
        if (!isSearching) {
            // No line detected, initiate a search
            if (previous_error < 0) {
                setMotorSpeed(-params.base, params.base);  // Turn left to search
            } else if (previous_error > 0) {
                setMotorSpeed(params.base, -params.base);  // Turn right to search
            }
            searchStartTime = millis();  // Record the start time
            isSearching = true;
        }

        // Check if the search time has elapsed (e.g., 200 ms)
        if (millis() - searchStartTime >= 200) {
            // Check again if all sensors still detect no line
            if (analogRead(sensor[0]) < params.threshold && analogRead(sensor[1]) < params.threshold &&
                analogRead(sensor[2]) < params.threshold && analogRead(sensor[3]) < params.threshold &&
                analogRead(sensor[4]) < params.threshold && analogRead(sensor[5]) < params.threshold) {
                isRunning = false;
                analogWrite(ENA, 0);
                analogWrite(ENB, 0); 
                onStop();  
                BluetoothPort.println("lost line");
            }
            isSearching = false;  // Reset the search flag
            return;  // Skip the rest of the loop
        }
    } else {
        isSearching = false;  // Reset the search flag if the line is found
    }

    // Detect crossing (if all or most sensors detect the line)
    if (analogRead(sensor[0]) < params.threshold && analogRead(sensor[1]) < params.threshold &&
        analogRead(sensor[2]) < params.threshold && analogRead(sensor[3]) < params.threshold &&
        analogRead(sensor[4]) < params.threshold && analogRead(sensor[5]) < params.threshold) {
        
        setMotorSpeed(params.base, params.base);  // Move straight ahead
        delay(500);  // Continue straight for a short time
        return;  // Skip the rest of the loop
    }

    //proportioneel
    correction = params.kp * error;

    //integraal
    iTerm += params.ki * error;
    iTerm = constrain(iTerm, -255, 255);
    correction += iTerm;
    
    //diferientiaal
    int lastError;
    correction += params.kd * (error - lastError);
    lastError = error;
    BluetoothPort.println(correction);
    //constraint
    correction = constrain(correction, -255, 255);
    
    // Set motor speeds based on correction
    int leftSpeed =  params.base + correction;
    if (leftSpeed <= 40 && leftSpeed >= -40 && leftSpeed > 0){
      leftSpeed = leftSpeed - 80;
    }
    else if (leftSpeed <= 40 && leftSpeed >= -40 && leftSpeed < 0){
      leftSpeed = leftSpeed + 80;
    }
    leftSpeed = constrain(leftSpeed, -params.max, params.max);
    int rightSpeed = params.base - correction;
     if (rightSpeed <= 40 && rightSpeed >= -40 && rightSpeed > 0){
      rightSpeed = rightSpeed - 80;
    }
    else if (rightSpeed <= 40 && rightSpeed >= -40 && rightSpeed < 0){
      rightSpeed = rightSpeed + 80;
    }   
    rightSpeed = constrain(rightSpeed, -params.max, params.max);

    //BluetoothPort.println(error);
    //BluetoothPort.print(leftSpeed);
    //BluetoothPort.println(" test");


    setMotorSpeed(leftSpeed, rightSpeed);

}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    BluetoothPort.print("start calibrating black... ");
    for (int i = 0; i < 6; i++) params.black[i]=analogRead(sensor[i]);
    SerialPort.println("done");
    BluetoothPort.println("done");    
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");
    BluetoothPort.print("start calibrating white... ");            
    for (int i = 0; i < 6; i++) params.white[i]=analogRead(sensor[i]);  
    SerialPort.println("done");
    BluetoothPort.println("done");    
      
  }

  EEPROM_writeAnything(0, params);
}