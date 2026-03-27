#ifndef SAFETY_SYSTEM_MANAGER_H
#define SAFETY_SYSTEM_MANAGER_H

#include <Arduino.h>
#include "PCF8575.h"

namespace SafetySystemManager {

struct SafetySystemContext {
  float systemTemp;
  float batteryTemp;
  float envT;
  float voltSensor;
  float batteryVoltageMain;
  float batteryVoltageSys;
  int rainSensor;
  int& rainDetectCount;
  long& systemTempCount;
  long& batteryTempCount;
  long& envTCount;
  long& voltHighCount;
  long& voltLowCount;
  long& batteryMainHighCount;
  long& batteryMainLowCount;
  long& batterySysHighCount;
  long& batterySysLowCount;
  long& fireDetectionCount;
  long& rainCounter;
  bool& sFlag1;
  bool& sFlag2;
  bool& sFlag3;
  bool& sFlag4;
  bool& sFlag5;
  bool& sFlag6;
  bool& sFlag7;
  bool& sFlag8;
  bool& sFlag9;
  bool& sFlag10;
  bool& rainProtect;
  String& logRain;
  const char* timestamp;
  PCF8575& pcf;
  int fanPin;
  int lightningProtectPin;
  int powerCutPin;
  void (*ledAllOff)();
  void (*addError)(const String& message);
};

void run(SafetySystemContext& context);

}  // namespace SafetySystemManager

#endif
