#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <Arduino.h>
#include "PCF8575.h"

namespace CommandManager {

struct CommandContext {
  String& cmd;
  PCF8575& pcf;
  float& voltSensor;
  int& pirCounter;
  bool& testV2cmd;
  bool& safetyMode;
  bool& cmdSystemAmp;
  bool& cmdRainSensor;
  bool& cmdSystemVolt;
  bool& cmdSysTemp;
  bool& cmdBatTemp;
  bool& cmdTestPir1;
  bool& cmdRstCount;
  bool& cmdTestPir2;
  bool& thunder;
  bool& localMidnightAutoLight;
  bool& localAutoLight;
  bool& alarmCutOff;
  bool& xModeEnabled;
  bool& xModeFlag;
  int alarmPin;
  int fanPin;
  int powerCutPin;
  int rf1Pin;
  int rf2Pin;
  int rf3Pin;
  int rf4Pin;
  int rfChannelPin;
  void (*ledAllOff)();
  void (*serialRead)();
};

void handleCommand(CommandContext& context, const String& input);

}  // namespace CommandManager

#endif
