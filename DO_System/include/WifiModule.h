#ifndef WIFI_MODULE_H //if not define
#define WIFI_MODULE_H

#include <DOSensorModule.h>

//  Define init fuction for Wifi module

// Init Wifi function
void initializeWiFi();
void connectWifi();
void disconnectWifi();

// Init Thingspeak function
void sendDataToThingSpeak(const SensorData& data);



#endif