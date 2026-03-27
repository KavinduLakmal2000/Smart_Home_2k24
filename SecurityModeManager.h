#ifndef SECURITY_MODE_MANAGER_H
#define SECURITY_MODE_MANAGER_H

#include <Arduino.h>
#include "PCF8575.h"

namespace SecurityModeManager {

struct SecurityModeContext {
  bool securityModeEnabled;
  int& pirCount;
  bool alarmCutOff;
  int insidePirPin;
  int outsidePirTopPin;
  PCF8575& pcf;
  int alarmPin;
  int rf1Pin;
  int rf2Pin;
  int rf3Pin;
  int rf4Pin;
  int rfChannelPin;
  int klroomChannel;
  int stairsChannel;
  int livingChannel;
  int diningChannel;
  int kitchenChannel;
  void (*ledAllOff)();
};

void run(SecurityModeContext& context);

}  // namespace SecurityModeManager

#endif
