#include <main.h>

// Send data to Thingspeak
// Thông tin mạng Wi-Fi

WiFiClient  client;

// const char* ssid = "TP-LINK_03"; // Thay thế bằng SSID của bạn
// const char* password = "12341234"; // Thay thế bằng mật khẩu Wi-Fi của bạn

const char* ssid = "LongBui"; // Thay thế bằng SSID của bạn
const char* password = "12345678"; // Thay thế bằng mật khẩu Wi-Fi của bạn

// Thông tin máy chủ ThingSpeak
unsigned long myChannelNumber = 4; // Thay thế bằng số kênh của bạn
const char* myWriteAPIKey = "O499F31QG9B7BFCC"; // Thay thế bằng Write API Key của kênh

//Init RTC and EEProm memory address
uRTCLib rtc(0x68);  // Create objects and assign module I2c Addresses
uEEPROMLib eeprom(0x57);

// Variable
char fileName[30];
float DOValue, TempValue;
unsigned long timer1, timer2;

char data_[40];
struct tm tmstruct;
struct
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} now_data;


// Init Oled
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define the sensor type
yosemitechModel model = Y504;  // The sensor model number

// Define the sensor's modbus address
byte modbusAddress = 0x01;  // The sensor's modbus address, or SlaveID
// Yosemitech ships sensors with a default ID of 0x01.

// Define pin number variables
const int PwrPin = 4;    // The pin sending power to the sensor *AND* RS485 adapter
const int DEREPin = -1;  // The pin controlling Recieve Enable and Driver Enable

const int SSRxPin = 16;  // Recieve pin for software serial (Rx on RS485 adapter)
const int SSTxPin = 17;  // Send pin for software serial (Tx on RS485 adapter)

// Construct software serial object for Modbus
SoftwareSerial modbusSerial(SSRxPin, SSTxPin);

// Construct the Yosemitech modbus instance
yosemitech sensor;
bool success;
