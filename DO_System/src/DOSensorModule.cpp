#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include "DOSensorModule.h"
#include <YosemitechModbus.h>  // Assuming use of a specific sensor library

// Construct the Yosemitech modbus instance
yosemitech sensor;
bool success;
// Define pin number variables
#define PwrPin  4    // The pin sending power to the sensor *AND* RS485 adapter
#define DEREPin -1  // The pin controlling Recieve Enable and Driver Enable

#define SSRxPin  16  // Recieve pin for software serial (Rx on RS485 adapter)
#define SSTxPin  17  // Send pin for software serial (Tx on RS485 adapter)

// Define the sensor type
yosemitechModel model = Y504;  // The sensor model number
// Define the sensor's modbus address
byte modbusAddress = 0x01;  // The sensor's modbus address, or SlaveID
// Yosemitech ships sensors with a default ID of 0x01.

// Construct software serial object for Modbus
SoftwareSerial modbusSerial(SSRxPin, SSTxPin);


// YosemitechModbus sensor;

void initializeSensors() {
    pinMode(PwrPin, OUTPUT);
    digitalWrite(PwrPin, HIGH);

    if (DEREPin > 0)
        pinMode(DEREPin, OUTPUT);

    modbusSerial.begin(9600); // The modbus serial stream - Baud rate MUST be 9600.

    // Start up the sensor
    sensor.begin(model, modbusAddress, &modbusSerial, DEREPin);

    // Turn on debugging
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

SensorData readSensorData() {
  SensorData data;
  // send the command to get the values
  data.doValue = -9999;
  data.tempValue = -9999;
  data.parmValue = -9999;
  // log data from sensor
  sensor.getValues(data.parmValue, data.tempValue, data.doValue);

  return data;
}
