const int sensorPin[] = {A0, A1, A2, A3, A4, A5}; 
unsigned long previousMillis = 0;
const long interval = 1000; 
void setup() {
  Serial.begin(9600); 
}

void loop() 
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    for (int i = 0; i < 6; i++) {
      int sensorValue = analogRead(sensorPin[i]);

      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(sensorValue);
    }
  }
}
