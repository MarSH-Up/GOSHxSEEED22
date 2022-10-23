#include <Arduino.h>
#include <SensirionI2CSht4x.h>
#include <Wire.h>
 
SensirionI2CSht4x sht4x;
uint16_t error;
char errorMessage[256];

void Init_SHT4X(){
  uint32_t serialNumber;
      error = sht4x.serialNumber(serialNumber);
      if (error) {
          Serial.print("Error trying to execute serialNumber(): ");
          errorToString(error, errorMessage, 256);
          Serial.println(errorMessage);
      } else {
          Serial.print("Serial Number: ");
          Serial.println(serialNumber);
      }  
}
float* Temp_Humid_extraction(){
  static float THvalues[2];
  float temperature;
  float humidity;
  error = sht4x.measureHighPrecision(temperature, humidity);
  if (error) {
    Serial.print("Error trying to execute measureHighPrecision(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } else {
    THvalues[0] = temperature;
    THvalues[1] = humidity;
    return THvalues;
  }
}
void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }
    Wire.begin();
    sht4x.begin(Wire);
    Init_SHT4X();
 
}
 
void loop() {

    float* TH = Temp_Humid_extraction();
    Serial.print("Temperature: ");
    Serial.println(TH[0]);
    Serial.print("Humidity: ");
    Serial.println(TH[1]);
    delay(1000);
    
}

