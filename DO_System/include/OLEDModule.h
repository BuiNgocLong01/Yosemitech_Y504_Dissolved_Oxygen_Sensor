#ifndef OLED_MODULE_H
#define OLED_MODULE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

extern Adafruit_SSD1306 display;


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)


void initializeOLED();
void displaySensorData(float temperature, float dissolvedOxygen);

#endif