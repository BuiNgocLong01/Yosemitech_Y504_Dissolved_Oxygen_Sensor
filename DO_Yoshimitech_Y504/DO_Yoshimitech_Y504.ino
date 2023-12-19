// ---------------------------------------------------------------------------
// The Yosemitech Y504 Dissolved Oxygen
// Author: Long Bui
// Date create: 19/12/2023
// ---------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <Wire.h>  // For the I2C for the OLED display
#include <Adafruit_GFX.h>  // For the OLED display
#include <Adafruit_SSD1306.h>  // For the OLED display
#include <YosemitechModbus.h>

// ---------------------------------------------------------------------------
// Set up the sensor specific information
//   ie, pin locations, addresses, calibrations and related settings
// ---------------------------------------------------------------------------

// Define the sensor type
yosemitechModel model = Y504;  // The sensor model number

// Define the sensor's modbus address
byte modbusAddress = 0x01;  // The sensor's modbus address, or SlaveID
// Yosemitech ships sensors with a default ID of 0x01.

// Define pin number variables
const int PwrPin = 10;  // The pin sending power to the sensor *AND* RS485 adapter
const int DEREPin = -1;   // The pin controlling Recieve Enable and Driver Enable
// on the RS485 adapter, if applicable (else, -1)
// Setting HIGH enables the driver (arduino) to send text
// Setting LOW enables the receiver (sensor) to send text
const int SSRxPin = 2;  // Recieve pin for software serial (Rx on RS485 adapter)
const int SSTxPin = 3;  // Send pin for software serial (Tx on RS485 adapter)

// Construct software serial object for Modbus
SoftwareSerial modbusSerial(SSRxPin, SSTxPin);

// Construct the Yosemitech modbus instance
yosemitech sensor;
bool success;

// ---------------------------------------------------------------------------
// Main setup function
// ---------------------------------------------------------------------------
void setup()
{

  pinMode(PwrPin, OUTPUT);
  digitalWrite(PwrPin, HIGH);

  if (DEREPin > 0) pinMode(DEREPin, OUTPUT);

  Serial.begin(57600);  // Main serial port for debugging via USB Serial Monitor
  Serial.println("Xin Chao Arduino Nano");
  modbusSerial.begin(9600);  // The modbus serial stream - Baud rate MUST be 9600.

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
  if (model == Y532)
  {
    byte status = sensor.pHCalibrationStatus();
    Serial.println("Calibration Status:");
    Serial.print("0x0");
    Serial.println(status, HEX);
    delay(3000);
  }

  // Get the sensor's current calibration values
  if (model != Y532)
  {
    float Kval = 0;
    float Bval = 0;
    sensor.getCalibration(Kval, Bval);
    Serial.println("Current Calibration Equation:");
    Serial.print(Kval);
    Serial.print("*raw + ");
    Serial.println(Bval);
    delay(3000);
  }

  if (model == Y511 || model == Y513 || model == Y514)
  {
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

  //  /*The modbus manuals recommend the following warm-up times between starting
  //    measurements and requesting values :
  //     2 s for whipered chlorophyll
  //     20 s for turbidity
  //     10 s for conductivity
  //
  //    On wipered (self-cleaning) models, the brush immediately activates after
  //    getting power and takes approximately 10-11 seconds to finish.  No
  //    readings should be taken during this time.
  //
  //    pH returns values after ~4.5 seconds
  //    Conductivity returns values after about 2.4 seconds, but is not stable
  //    until ~10 seconds.
  //    DO does not return values until ~8 seconds
  //    Turbidity takes ~22 seconds to get stable values.
  //  */
  
    Serial.println("Allowing sensor to stabilize..");
  //
  if (model == Y511 || model == Y513 || model == Y514)
  {

    Serial.println("Activating brush.");
    success = sensor.activateBrush();
    if (success) 
    Serial.println("    Brush activated.");
    else 
    Serial.println("    Failed to activate brush!");
  }
    if (model == Y511 || model == Y513 || model == Y514 || model == Y510)
    {
      Serial.println("Continuing to stabilize..");
    }
}

// ---------------------------------------------------------------------------
// Main loop function
// ---------------------------------------------------------------------------
void loop()
{

  // send the command to get the values
  float parmValue, tempValue, thirdValue = -9999;
  
  sensor.getValues(parmValue, tempValue, thirdValue);
  
  Serial.print("Do Value: "); Serial.println(thirdValue);
  Serial.println("-----------------");
  Serial.print("Temperature Value: "); Serial.print(tempValue);




  /*Delay between readings
    Modbus manuals recommend the following re-measure times:
      2 s for chlorophyll
      2 s for turbidity
      3 s for conductivity
      1 s for DO
    The turbidity and DO sensors appear return new readings about every 1.6 seconds.
    The pH sensor returns new readings about every 1.8 seconds.
    The conductivity sensor only returns new readings about every 2.7 seconds.
    The teperature sensors can take readings much more quickly.  The same results
    can be read many times from the registers between the new sensor readings.
  */
  delay(1700);
}
