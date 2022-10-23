#include "rpcWiFi.h"
 
//const char* ssid = "INFINITUM2F24_2.4_EXT";
//const char* password =  "RN5sDQTMt7";

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

void setup() {
    Serial.begin(115200);
    while(!Serial); 
    Wifi_WioTerminal("INFINITUM2F24_2.4_EXT", "RN5sDQTMt7");
  
}
 
void loop() {
 
}
