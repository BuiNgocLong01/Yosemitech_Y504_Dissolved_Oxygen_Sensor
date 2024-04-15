#include <main.h>

// Bluetooth Serial object
// BluetoothSerial SerialBT;

// Send data to Thingspeak
// Thông tin mạng Wi-Fi

WiFiClient  client;

// const char* ssid = "TP-LINK_03"; // Thay thế bằng SSID của bạn
// const char* password = "12341234"; // Thay thế bằng mật khẩu Wi-Fi của bạn

const char* ssid = "Mo sap map"; // Thay thế bằng SSID của bạn
const char* password = "moditbuhihi"; // Thay thế bằng mật khẩu Wi-Fi của bạn

// Thông tin máy chủ ThingSpeak
unsigned long myChannelNumber = 4; // Thay thế bằng số kênh của bạn
const char* myWriteAPIKey = "O499F31QG9B7BFCC"; // Thay thế bằng Write API Key của kênh


//Init RTC and EEProm memory address
uRTCLib rtc(0x68);  // Create objects and assign module I2c Addresses
uEEPROMLib eeprom(0x57);

// Variable
char fileName[40];

char data_[50];
struct tm tmstruct;
struct
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
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
float DOvalue, temp;

// Define pin number variables
#define PwrPin  4    // The pin sending power to the sensor *AND* RS485 adapter
#define DEREPin -1  // The pin controlling Recieve Enable and Driver Enable

#define SSRxPin  16  // Recieve pin for software serial (Rx on RS485 adapter)
#define SSTxPin  17  // Send pin for software serial (Tx on RS485 adapter)

// Construct software serial object for Modbus
SoftwareSerial modbusSerial(SSRxPin, SSTxPin);

// Construct the Yosemitech modbus instance
yosemitech sensor;
bool success;

unsigned long timer1, timer2, timer3;
