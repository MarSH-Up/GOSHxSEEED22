int SoilSensor = A0; //Port
int SensorValue = 0; //To save the value
void setup() {
    Serial.begin(9600); //We want to see the value, right?
}

void loop() {
 
 SensorValue = analogRead(SoilSensor);
  //Print the value each second
 Serial.print("Moisture = ");
 Serial.println(SensorValue);
 delay(1000);

}



