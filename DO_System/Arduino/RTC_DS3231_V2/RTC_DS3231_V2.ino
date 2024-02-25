#include "Arduino.h"
#include "Wire.h"
#include "uRTCLib.h"
#include "uEEPROMLib.h"

uRTCLib rtc(0x68);  // Create objects and assign module I2c Addresses
uEEPROMLib eeprom(0x57);

unsigned int addr_EEPROM;  // Variable to store current EEPROM memory address
//===============================================================================
//  Initialization
//===============================================================================
void setup()
 {
  Serial.begin(9600);
  Wire.begin();

}
//===============================================================================
//  Main
//===============================================================================
void loop() 
{
  rtc.refresh();

  Serial.print("Date: ");
  Serial.print(rtc.month());
  Serial.print('/');
  Serial.print(rtc.day());
  Serial.print('/');
  Serial.print(rtc.year());

  Serial.print("  Time: ");
  Serial.print(rtc.hour());
  Serial.print(':');
  Serial.print(rtc.minute());
  Serial.print(':');
  Serial.print(rtc.second());
  Serial.print(" ");

  Serial.print(DayAsString(rtc.dayOfWeek()));

  Serial.print("  Current Temp: ");
  Serial.println(rtc.temp() / 100);

  delay(1000);    // Pause between updates

}
//===============================================================================
//  DayAsString Function:  Accepts 1-7, Returns the day of the week as string
//===============================================================================
String DayAsString(int day) {
  switch (day) {
    case 1: return "Sunday";
    case 2: return "Monday";
    case 3: return "Tuesday";
    case 4: return "Wednesday";
    case 5: return "Thursday";
    case 6: return "Friday";
    case 7: return "Saturday";
  }
  return "(Incorrect Day)";
}