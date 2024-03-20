# Yosemitech_Y504_Dissolved_Oxygen_Sensor
1. DO_System folder: Using ESP32
   - Block Diagram
   ![DO_SYStem](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/30d05209-8912-4521-9fb7-692ca1b1d1aa)
  - Function:
    -	Collect DO and temperature data from sensors via RS485 mudule,
    -	Calibrate data and save to SD card,
    -	Get real time from RTC DS3231 module,
    -	Display data on OLED screen,
    -	Send data to Thingspeak for remote monitoring via ESP32 module and WIFI.
   - Connection diagram: ![DO_CONNECTION](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/264f7f94-e82c-4a02-aa19-2d17274fbac1)
   - Pinout: ![Pinout_DO](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/471e5daa-a1bd-4826-a217-421e0b83cb47)
   - Results:
     - ThingSpeak API: ![image](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/fd6e67b0-ce73-49bb-8496-409ef9da1cb6)
     - SD Card: ![z5267207482797_119ae0d6ab951a3f3df69892cb50034c](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/a74cc89e-b1f6-4471-b233-989142e78bc7)


2. DO System using Arduino Nano
- Video tutorial on how to use UART to RS485 converter: https://youtu.be/KCNV3e-zXdE
- Reference:
  + Yosemitech Y504 Dissolved Oxygen Sensor: https://envirodiy.github.io/ModularSensors/group__sensor__y504.html#sensor_y504_examples
  + https://envirodiy.github.io/ModularSensors/group__sensor__y504.html
- Hardware: Arduino Nano Atmega328, UART TTL to RS485 Converter v2, SD Card(SPI)
- Connection diagram - USB to RS485: ![UART_RS485_CONVERTER](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/d759db0a-2ccd-4c3c-bc3a-fd8efb83692e)
- Connection diagram - RS495 Sensor: ![Connection_DO_Sensor](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/37b346ac-12d2-4c7d-86f0-4d6619a82492)
- Sensor: Yosemitech Y504 Dissolved Oxygen Sensor 
![DO_SENSOR](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/64eb4f3e-d37b-4299-97f0-2fba5e45c363)
- Result: ![image](https://github.com/BuiNgocLong01/Yosemitech_Y504_Dissolved_Oxygen_Sensor/assets/93063745/f1215d36-1407-4583-974d-7fed50f27f1b)

