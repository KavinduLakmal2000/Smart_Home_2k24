#ifndef IR_SWITCH_MANAGER_H
#define IR_SWITCH_MANAGER_H

#include <Arduino.h>
#include "PCF8575.h"

namespace IRSwitchManager {

struct IRSwitchContext {
  String& irSensor;
  bool& flag5;
  PCF8575& pcf;
  int rfChannelPin;
  int rf3Pin;
};

void run(IRSwitchContext& context);

}  // namespace IRSwitchManager

#endif
