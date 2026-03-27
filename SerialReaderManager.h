#ifndef SERIAL_READER_MANAGER_H
#define SERIAL_READER_MANAGER_H

#include <Arduino.h>

namespace SerialReaderManager {

struct SerialReaderContext {
  float& voltSensor;
  float& ampSensor;
  float& microControllerVoltAnalog;
  float& batteryVoltageMain;
  float& powerW;
  String& irSensor;
  bool thunder;
  int& rainSensor;
  long& rainCounter;
  float maxSystemBatteryVoltage;
  int maxADCValue;
  float& batteryVoltageSys;
};

void run(SerialReaderContext& context);

}  // namespace SerialReaderManager

#endif
