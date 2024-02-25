/*
   DS3231 RTC and EEPROM Test

   This program exercises the DS3231 RTC module with built-in EEPROM
   It tests the EEPROM first 7 bytes and then writes the value 0-255 to the entire EEPROM
   It then outputs the date, time, temperature and EEPROM values by address to the
   Serial Monitor Window.

   Connect SDA to SDA and SCL to SCL.  VCC connects to 3.3 or 5V, GND to ground

   Need to install 'uRTCLib.h' and uEEPROMLib libraries.  

   If running program for first time, uncomment these line and enter the correct day/time info
      // rtc.set(0, 39, 13, 6, 19, 10, 18);
   Once correct date/time info has been entered, comment this line back out.
*/

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
void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Use following command once to set current day/time, then disable by commenting it out.
  // rtc.set(0, 45, 19, 1, 25, 2, 24);
  // Format:  Seconds(0-59), Minute(0-59), Hour(0-23), Day of Week - Sun thru Sat (1-7),
  // Day of Month(1-31), Month(1-12), Year(00-99)

  // // Below are dummy values to test the EEPROM write/read with
  // char char_temp = 'A';
  // int int_temp = 32123;
  // float float_temp = 3.1416;

  // // Test EEPROM Write/Read Functionality
  // Serial.print("Writing CHAR: ");  // Test Char value (1 byte)
  // Serial.println(char_temp);
  // if (!eeprom.eeprom_write(0, char_temp)) {
  //   Serial.println("Failed to store CHAR");
  // } else {
  //   Serial.println("CHAR correctly stored");
  // }
  // Serial.println();

  // Serial.print("Writing INT: ");  // Test Integer value (2 bytes)
  // Serial.print(int_temp);
  // if (!eeprom.eeprom_write(1, int_temp)) {
  //   Serial.println("Failed to store INT");
  // } else {
  //   Serial.println("INT correctly stored");
  // }
  // Serial.println();

  // Serial.print("Writing FLOAT: ");  // Test Floating Point (4 bytes)
  // Serial.print(float_temp, 4);
  // if (!eeprom.eeprom_write(3, float_temp)) {
  //   Serial.println("Failed to store FLOAT");
  // } else {
  //   Serial.println("FLOAT correctly stored");
  // }
  // Serial.println();

  // char_temp = 0;  // Clear the variables and reinitialize with values read from EEPROM
  // int_temp = 0;
  // float_temp = 0;

  // Serial.println("Reading Values Back Out of EEPROM");
  // Serial.print("CHAR: ");
  // eeprom.eeprom_read(0, &char_temp);
  // Serial.println(char_temp);
  // Serial.print("INT: ");
  // eeprom.eeprom_read(1, &int_temp);
  // Serial.println(int_temp);
  // Serial.print("FLOAT: ");
  // eeprom.eeprom_read(3, &float_temp);
  // Serial.println((float)float_temp, 4);
  // Serial.println();

  // Serial.println("Writing the values ranging from 0-255 into the entire EEPROM");
  // Serial.println("starting at location 7.  This will take a few moments");
  // for (addr_EEPROM = 7; addr_EEPROM < 4096; addr_EEPROM++) {
  //   eeprom.eeprom_write(addr_EEPROM, (unsigned char)(addr_EEPROM % 256));
  //   if (int i = addr_EEPROM % 100 == 0) Serial.print(".");  // Prints a '.' every 100 writes to EEPROM
  // }
  // Serial.println();
  addr_EEPROM = 0;
}
//===============================================================================
//  Main
//===============================================================================
void loop() {
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
  Serial.print(rtc.temp() / 100);

  Serial.print("   EEPROM Addr/Data ");
  Serial.print(addr_EEPROM);
  Serial.print(": ");
  Serial.print(eeprom.eeprom_read(addr_EEPROM));
  Serial.println();

  addr_EEPROM++;  // Increment memory location
  delay(1000);    // Pause between updates

  // // We are just going to check the first 10 addresses, then clear the memory
  // // Address 0-6 will contain the Char, Int and Float data. 7 thru 10 should
  // // contain the numbers 7-10
  // if (addr_EEPROM >= 11) {
  //   Serial.println("Clearing the entire EEPROM.  This will take a few moments");
  //   for (addr_EEPROM = 0; addr_EEPROM < 4096; addr_EEPROM++) {
  //     eeprom.eeprom_write(addr_EEPROM, (unsigned char)(0));
  //     if (int i = addr_EEPROM % 100 == 0) Serial.print(".");  // Prints a '.' every 100 writes to EEPROM
  //   }
  //   Serial.println();
  //   Serial.println("Memory Erase Complete");
  //   while (1)
  //     ;  //Stops further execution of the program
  // }
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