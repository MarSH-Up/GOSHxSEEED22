/* 
%% Version V1
% Author - Mario De Los Santos
% Date - October 22th, 2022
% Email - madlsh3517@gmail.com
% Description; 
%
%   The code simplify the data collecyion of multiple sensor of the SenseCAP K1100. 
%   Each function its independent and its possible to take them to any other code. The
%   functions are divided as:
%     1. Wifi Connection: (Wio Terminal)
%            - Wifi_WioTerminal(const char* ssid, const char* password)
%     2. Sensor: 
%            - Temp and Humid sensor:
%                     - Init_SHT4X()
%                     - Temp_Humid_extraction()
%            - VOC and eCOS2
%     3. Ubisoft connection
%           - callback(char* topic, byte* payload, unsigned int length)
%           - reconnect()
%           - Publish_Ubidots(float data, char Variable[10])
*/
//
//
// LIBRARIES
#include "rpcWiFi.h"
#include <PubSubClient.h>
#include <Arduino.h>
#include "sensirion_common.h"
#include "sgp30.h"
#include <SensirionI2CSht4x.h>
#include <Wire.h>

// UBIDOTS Credentials
#define TOKEN "" // Put your Ubidots' TOKEN
#define DEVICE_LABEL "wioterminal" // Assign the device label
#define MQTT_CLIENT_NAME "MdlsSeeed02" // MQTT client Name (random name)
char mqttBroker[] = "industrial.api.ubidots.com";

// WIFI CLIENTS
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Sensor temp and humid
SensirionI2CSht4x sht4x;
uint16_t error;
char errorMessage[256];

void Wifi_WioTerminal(const char* ssid, const char* password){
  WiFi.mode(WIFI_STA);
      WiFi.disconnect();
  
      Serial.println("Connecting to WiFi..");
      WiFi.begin(ssid, password);

      int Testing = 1;
      while (WiFi.status() != WL_CONNECTED) {
          Serial.println("Connecting to WiFi..");
          WiFi.begin(ssid, password);

          Testing++;
          if(Testing >= 10)
            break;
        
      }
      if (Testing < 10){
        Serial.println("Connected to the WiFi network");
        Serial.print("IP Address: ");
        Serial.println (WiFi.localIP()); 
      }
      else if(Testing >= 10){
        Serial.print("Unable to connect");
      }
}

void callback(char* topic, byte* payload, unsigned int length){

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

  // Attempt to connect
  if (client.connect(MQTT_CLIENT_NAME, TOKEN,"")) {
    Serial.println("connected");
  }
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 2 seconds");
    // Wait 2 seconds before retrying
    delay(2000);
    }
  }
}

void Publish_Ubidots(float data, char Variable[10]){
  // Space to store values to send
  char str_data[10];
  char payload[700];
  char topic[150];
  dtostrf(data, 9, 1, str_data);

  // Builds the topic
  sprintf(topic, "%s", ""); // Cleans the topic content
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);

  //Builds the payload
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", Variable); // Adds the variable label 
  sprintf(payload, "%s%s", payload, str_data); // Adds the value
  sprintf(payload, "%s}", payload); // Closes the dictionary brackets
  client.publish(topic, payload);
  Serial.println(payload);
}
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
  Serial.begin(9600);
  Wifi_WioTerminal("SSID", "password"); 
  pinMode(WIO_LIGHT, INPUT);

  while(!Serial); 
  Wire.begin();
  sht4x.begin(Wire);
  Init_SHT4X();
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

}

void loop() {
  float light = analogRead(WIO_LIGHT);
  float* TH = Temp_Humid_extraction();
  if (!client.connected()) {
    reconnect();
  }
  Publish_Ubidots(light, "light");
  Publish_Ubidots(TH[0], "Temp");
  Publish_Ubidots(TH[1], "Humid");
  delay(5000);
  client.loop();
}

