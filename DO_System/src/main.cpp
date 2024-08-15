
/**
 * @file main.cpp
 * @author Bùi Ngọc Long
 * @date 2024-04-17
 * @brief Short description of what this file does.
 *
 * Long description if needed...
 * 
 * @license MIT License
 */

#include <Arduino.h>
#include <OLEDModule.h>
#include <RTCModule.h>
#include <SDCardModule.h>
#include <DOSensorModule.h>
#include <WifiModule.h>

unsigned long previousMillis_1 = 0;
unsigned long previousMillis_2 = 0;
unsigned long previousMillis_3 = 0; 
const long interval = 60000;  // 2 phút 1 data


void setup() {
  Serial.begin(115200);
    
  //Init
  initializeSensors();
  initializeSDCard();
  initializeRTC();
  initializeWiFi();
  initializeOLED();
}


void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis_1 >= interval) {
    previousMillis_1 = currentMillis;

    SensorData data = readSensorData();
    Serial.print("Do Value: ");    Serial.println(data.doValue);
    Serial.print("Temperature Value: ");    Serial.println(data.tempValue);
    Serial.println("------------------------");

    //Test 
    if(data.doValue > 8 || data.doValue < 3 || data.tempValue < 25 || data.tempValue > 40)
    {
      connectWifi();
      sendDataToThingSpeak(data);
      disconnectWifi();
    }

    // Get Date Time
    DateTime now = getCurrentDateTime();
    Serial.printf("Date time is : %d-%02d-%02d %02d:%02d:%02d", now.year + 2000, now.month, now.day, now.hour, now.minute, now.second);
    

    char fileName[50], dataString[50];
    sprintf(fileName, "/%04d-%02d-%02d.txt", now.year+2000, now.month, now.day);
    sprintf(dataString, "%04d-%02d-%02d,%02d-%02d-%02d,%0.02f,%0.02f",
            now.year + 2000, now.month, now.day, now.hour, now.minute, now.second,
            data.doValue, data.tempValue);
    
    Serial.print("\nFile name is: ");    Serial.println(fileName);
    Serial.print("Data content: ");    Serial.println(dataString); 

    // Kiểm tra và tạo file mới nếu cần
    checkAndCreateFile(fileName);

    // Ghi dữ liệu vào file
    appendToFile(fileName, dataString);
    Serial.println("------------------------------------------");
  }

  // Hien thi tren Oled
  if (currentMillis - previousMillis_3 >= 5000) {
    previousMillis_3 = currentMillis;
    SensorData data = readSensorData();

    // Hien thi tren Oled
    displaySensorData(data.tempValue, data.doValue);

  }

  if ((unsigned long)(millis() - previousMillis_2) > 30000)  { // 2 phút 1 dữ liệu lên Thingspeak
    previousMillis_2 = millis(); 

    SensorData data = readSensorData();
    connectWifi();
    sendDataToThingSpeak(data);
    disconnectWifi();
  }
}

