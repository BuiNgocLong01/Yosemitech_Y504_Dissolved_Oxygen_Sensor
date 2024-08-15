#ifndef DO_SENSOR_MODULE_H
#define DO_SENSOR_MODULE_H


struct SensorData {
  float doValue;
  float tempValue;
  float parmValue;
};

void initializeSensors();

SensorData readSensorData();


#endif