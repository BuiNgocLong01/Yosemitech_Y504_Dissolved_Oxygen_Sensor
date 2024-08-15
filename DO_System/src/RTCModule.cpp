#include "RTCModule.h"
#include <uRTCLib.h>
#include "uEEPROMLib.h"

//Init RTC and EEProm memory address
uRTCLib rtc(0x68);  // Create objects and assign module I2c Addresses
uEEPROMLib eeprom(0x57);

void initializeRTC() {
  rtc.refresh();
}

DateTime getCurrentDateTime() {
  rtc.refresh();
  DateTime now = {
    rtc.year(),
    rtc.month(),
    rtc.day(),
    rtc.hour(),
    rtc.minute(),
    rtc.second()
  };
  return now;
}
