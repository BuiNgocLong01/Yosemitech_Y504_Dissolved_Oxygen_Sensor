// ---------------------------------------------------------------------------
// The Yosemitech Y504 Dissolved Oxygen
// Author: Long Bui
// Date create: 25/02/2024
// ---------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <Wire.h>              // For the I2C for the OLED display
#include <Adafruit_GFX.h>      // For the OLED display
#include <Adafruit_SSD1306.h>  // For the OLED display
#include <YosemitechModbus.h>
#include <Adafruit_Sensor.h>

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
float DOValue, TempValue;

unsigned long timer1, timer2;

// ---------------------------------------------------------------------------
// Main setup function
// ---------------------------------------------------------------------------
void setup() {

  pinMode(PwrPin, OUTPUT);
  digitalWrite(PwrPin, HIGH);

  if (DEREPin > 0) pinMode(DEREPin, OUTPUT);

  Serial.begin(57600);  // Main serial port for debugging via USB Serial Monitor
  Serial.println("Xin Chao Arduino Nano");
  modbusSerial.begin(9600);  // The modbus serial stream - Baud rate MUST be 9600.

  //millis
  timer1 = millis();
  timer2 = millis();

  //Oled
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);


  // Start up the sensor
  sensor.begin(model, modbusAddress, &modbusSerial, DEREPin);

  //  // Turn on debugging
  sensor.setDebugStream(&Serial);
  //
  //  // Start up note
  Serial.println("Yosemitech ");
  Serial.println(sensor.getModel());
  Serial.println(sensor.getParameter());
  Serial.println("Sensor");

  float hardwareV, softwareV;
  sensor.getVersion(hardwareV, softwareV);
  Serial.println("Hardware Version:");
  Serial.println(hardwareV);
  Serial.println("Software Version:");
  Serial.println(softwareV);

  //  // Get the sensor serial number
  String SN = sensor.getSerialNumber();
  Serial.println("Serial Number:");
  Serial.print(SN);
  delay(3000);

  // Get the sensor calibration status (pH only)
  if (model == Y532) {
    byte status = sensor.pHCalibrationStatus();
    Serial.println("Calibration Status:");
    Serial.print("0x0");
    Serial.println(status, HEX);
    delay(3000);
  }

  // Get the sensor's current calibration values
  if (model != Y532) {
    float Kval = 0;
    float Bval = 0;
    sensor.getCalibration(Kval, Bval);
    Serial.println("Current Calibration Equation:");
    Serial.print(Kval);
    Serial.print("*raw + ");
    Serial.println(Bval);
    delay(3000);
  }

  if (model == Y511 || model == Y513 || model == Y514) {
    // Check the wiper timing
    uint16_t interval = sensor.getBrushInterval();
    Serial.println("Sensor auto-cleaning interval: ");
    Serial.print(interval);
    Serial.println(" minutes");
    delay(3000);
  }

  Serial.println("Starting sensor measurements");
  success = sensor.startMeasurement();
  if (success)
    Serial.println("    Measurements started.");
  else
    Serial.println("    Failed to start measuring!");

  Serial.println("Allowing sensor to stabilize..");
  //
  if (model == Y511 || model == Y513 || model == Y514) {

    Serial.println("Activating brush.");
    success = sensor.activateBrush();
    if (success)
      Serial.println("    Brush activated.");
    else
      Serial.println("    Failed to activate brush!");
  }
  if (model == Y511 || model == Y513 || model == Y514 || model == Y510) {
    Serial.println("Continuing to stabilize..");
  }
}

// ---------------------------------------------------------------------------
// Main loop function
// ---------------------------------------------------------------------------
void loop() {

  // log data from sensor
  if ((unsigned long)(millis() - timer1) > 1700) {

    // send the command to get the values
    float parmValue, tempValue, thirdValue = -9999;

    sensor.getValues(parmValue, tempValue, thirdValue);
    DOValue = thirdValue;
    TempValue = tempValue;

    Serial.print("Do Value: ");
    Serial.println(thirdValue);
    Serial.println("-----------------");
    Serial.print("Temperature Value: ");
    Serial.print(tempValue);
    timer1 = millis();
  }

  // Print Data to Oled
  if ((unsigned long)(millis() - timer2) > 1000) {
    // clear display
    display.clearDisplay();
    // display DO Value
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("DO Value: ");
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print(DOValue);
    display.print(" mg/l");

    // display Temperature
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Temparature: ");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print(TempValue);
    display.print(" ");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.setTextSize(2);
    display.print("C");

    display.display();
    timer2 = millis();
  }
}