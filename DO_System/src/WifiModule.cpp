#include "WiFiModule.h"
#include <WiFi.h>
#include <ThingSpeak.h>

// Config Wifi
const char* ssid = "VGU_AQI_1";
const char* password = "myPMS5003";

// const char* ssid = "LongBui";
// const char* password = "12345678";


//Config Thingspeak
unsigned long channelID = 1;
const char* apiKey = "O499F31QG9B7BFCC";

WiFiClient client;


void initializeWiFi() {
  //
  WiFi.mode(WIFI_OFF);
}

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  Serial.print("Connecting to Wi-Fi");

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) { //sau 10 giay connect khong duoc thi bo qua
    delay(500);
    Serial.print(".");

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected.");
    }
  }
}

void disconnectWifi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected.");
}

void sendDataToThingSpeak(const SensorData& data) {
  ThingSpeak.setField(1, data.doValue);
  ThingSpeak.setField(2, data.tempValue);
  delay(200);
  ThingSpeak.writeFields(channelID, apiKey);
  //  int result = ThingSpeak.writeFields(channelID, apiKey);
  // if (result == 200) {
  //   Serial.println("Data sent to ThingSpeak successfully!");
  // } else {
  //   Serial.println("Failed to send data to ThingSpeak. Error code: " + String(result));
  // }
}
