// ---------------------------------------------------------------------------
// The Yosemitech Y504 Dissolved Oxygen
// Author: Long Bui
// Date create: 25/02/2024
// ---------------------------------------------------------------------------

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>              // For the I2C for the OLED display
#include <Adafruit_GFX.h>      // For the OLED display
#include <Adafruit_SSD1306.h>  // For the OLED display
#include <YosemitechModbus.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <ThingSpeak.h>


// SPI: SD Card
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <time.h>

// RTC
#include "uRTCLib.h"
#include "uEEPROMLib.h"
