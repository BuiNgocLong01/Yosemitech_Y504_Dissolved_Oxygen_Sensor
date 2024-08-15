#ifndef RTC_MODULE_H
#define RTC_MODULE_H


struct DateTime {
  int year, month, day, hour, minute, second;
};

void initializeRTC();

DateTime getCurrentDateTime();


#endif