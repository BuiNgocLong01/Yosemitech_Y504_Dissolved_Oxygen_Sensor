#ifndef SD_CARD_MODULE_H
#define SD_CARD_MODULE_H

#include <RTCModule.h>
#include <DOSensorModule.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <time.h>
#include <Wire.h>



void initializeSDCard();

void checkAndCreateFile(const char* fileName);
void appendToFile(const char* fileName, const char* data);


#endif