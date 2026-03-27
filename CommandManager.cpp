#include "CommandManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>
#include "SDCardManager.h"
#include "esp_system.h"

namespace CommandManager {

namespace {

void sendStatus(const String& message) {
  Blynk.virtualWrite(V2, message);
}

void setSingleShotFlag(bool& target, const String& cmd, const char* expected) {
  target = (cmd == expected);
}

void sendHelp() {
  sendStatus("📖 Command Help Menu:");
  delay(100);

  sendStatus("pCut - Cut main power temporarily");
  sendStatus("test - Enable test mode");
  sendStatus("sftyOn / sftyOff - Safety mode ON/OFF");
  sendStatus("getAmps - Get system current");
  sendStatus("getRain - Get rain sensor status");
  sendStatus("getSystemVolt - Get system voltage");
  sendStatus("getSystemTemp - Get system temperature");
  sendStatus("getBatteryTemp - Get battery temperature");
  sendStatus("testInPir / testOutTopPir - PIR test modes");
  sendStatus("rstCount - Reset counter");
  sendStatus("ELightOff - Emergency lights OFF");
  sendStatus("chkSD - Check SD card space");
  sendStatus("sftyLog - Send safety logs");
  sendStatus("firmLog - Send firmware log");
  sendStatus("autoLightOff - Force auto light OFF");
  sendStatus("about - Show about info");
  sendStatus("almLocOn / almLocOff - Lock/Unlock alarm");
  sendStatus("almOn / almOff - Turn alarm ON/OFF");
  sendStatus("pwrGon / pwrGoff - Lightning protection ON/OFF");
  sendStatus("fan - Test fan");
  sendStatus("reset - Restart ESP32");
  sendStatus("rf1On / rf1Off ... rf4On / rf4Off - RF Switch control");
  sendStatus("rfCH1 / rfCH2 - Select RF Channel");
  sendStatus("rfAllOn / rfAllOff - RF lights ON/OFF");
  sendStatus("rfStatus - Get RF status report");
}

void sendRfSwitchState(PCF8575& pcf, int pin, const char* label) {
  if (pcf.digitalRead(pin)) {
    sendStatus(String(label) + " OFF");
  } else {
    sendStatus(String(label) + " ON");
  }
}

void sendRfStatus(PCF8575& pcf, int rfChannelPin, int rf1Pin, int rf2Pin, int rf3Pin, int rf4Pin) {
  sendStatus("RF Status Report");
  delay(100);

  if (pcf.digitalRead(rfChannelPin)) {
    sendStatus("RF Channel 1 (Ground Floor)");
  } else {
    sendStatus("RF Channel 2 (Mid Floor)");
  }

  delay(100);
  sendRfSwitchState(pcf, rf1Pin, "RF SW1");
  delay(100);
  sendRfSwitchState(pcf, rf2Pin, "RF SW2");
  delay(100);
  sendRfSwitchState(pcf, rf3Pin, "RF SW3");
  delay(100);
  sendRfSwitchState(pcf, rf4Pin, "RF SW4");
}

void setRfAll(PCF8575& pcf, int rf1Pin, int rf2Pin, int rf3Pin, int rf4Pin, int rfChannelPin, bool on) {
  int state = on ? LOW : HIGH;
  pcf.digitalWrite(rf1Pin, state);
  pcf.digitalWrite(rf2Pin, state);
  pcf.digitalWrite(rf3Pin, state);
  pcf.digitalWrite(rf4Pin, state);
  delay(500);
  pcf.digitalWrite(rfChannelPin, HIGH);
  delay(500);
  pcf.digitalWrite(rfChannelPin, LOW);
  sendStatus(on ? "All RF lights ON" : "All RF lights OFF");
}

}  // namespace

void handleCommand(CommandContext& context, const String& input) {
  context.cmd = input;

  if (context.cmd.startsWith("#")) {
    Serial.print(context.cmd);
  }

  if (context.cmd == "pCut") {
    context.pcf.digitalWrite(context.powerCutPin, LOW);
    delay(1000);
    context.pcf.digitalWrite(context.powerCutPin, HIGH);
    context.serialRead();
    delay(1000);
    if (context.voltSensor < 30) {
      sendStatus("Main Power is off!");
    } else {
      sendStatus("Power Cut-off is faild!");
    }
  }

  context.testV2cmd = (context.cmd == "test");

  if (context.cmd == "sftyOn") {
    context.safetyMode = true;
  }
  if (context.cmd == "sftyOff") {
    context.safetyMode = false;
  }

  setSingleShotFlag(context.cmdSystemAmp, context.cmd, "getAmps");
  setSingleShotFlag(context.cmdRainSensor, context.cmd, "getRain");
  setSingleShotFlag(context.cmdSystemVolt, context.cmd, "getSystemVolt");
  setSingleShotFlag(context.cmdSysTemp, context.cmd, "getSystemTemp");
  setSingleShotFlag(context.cmdBatTemp, context.cmd, "getBatteryTemp");
  setSingleShotFlag(context.cmdTestPir1, context.cmd, "testInPir");
  setSingleShotFlag(context.cmdRstCount, context.cmd, "rstCount");
  setSingleShotFlag(context.cmdTestPir2, context.cmd, "testOutTopPir");

  if (context.cmd == "offRain") {
    sendStatus("Automatic lightning protection off!");
    context.thunder = false;
  }

  if (context.cmd == "onRain") {
    sendStatus("Automatic lightning protection on!");
    context.thunder = true;
  }

  if (context.cmd == "ELightOff") {
    context.pirCounter = 0;
    context.ledAllOff();
  }

  if (context.cmd == "Xauto") {
    if (SDCardManager::setSetting("Xmode", HIGH)) {
      sendStatus("New settings saved!");
    } else {
      sendStatus("Failed to save new settings");
    }
  }

  if (context.cmd == "Xmanual") {
    if (SDCardManager::setSetting("Xmode", LOW)) {
      sendStatus("New settings saved!");
    } else {
      sendStatus("Failed to save new settings");
    }
  }

  if (context.cmd == "MidNightAutoLightOn") {
    if (SDCardManager::setSetting("m_AutoLight", HIGH)) {
      sendStatus("New settings saved!");
      context.localMidnightAutoLight = true;
    } else {
      sendStatus("Failed to save new settings");
    }
  }

  if (context.cmd == "MidNightAutoLightOff") {
    if (SDCardManager::setSetting("m_AutoLight", LOW)) {
      sendStatus("New settings saved!");
      context.localMidnightAutoLight = false;
    } else {
      sendStatus("Failed to save new settings");
    }
  }

  if (context.cmd == "AutoLightOn") {
    if (SDCardManager::setSetting("AutoLight", HIGH)) {
      sendStatus("New settings saved!");
      context.localAutoLight = true;
    } else {
      sendStatus("Failed to save new settings");
    }
  }

  if (context.cmd == "AutoLightOff") {
    if (SDCardManager::setSetting("AutoLight", LOW)) {
      sendStatus("New settings saved!");
      context.localAutoLight = false;
    } else {
      sendStatus("Failed to save new settings");
    }
  }

  if (context.cmd == "readSettings") {
    SDCardManager::sendSettings();
  }

  if (context.cmd == "chkSD") {
    SDCardManager::checkCardSpace();
  }

  if (context.cmd == "sftyLog") {
    SDCardManager::sendLastLinesFromFile("/Safety_system_Error_log.csv", "Error Log");
    SDCardManager::sendLastLinesFromFile("/Safety_system_Status_log.csv", "Status Log");
  }

  if (context.cmd == "firmLog") {
    SDCardManager::sendFirmwareLog();
  }

  if (context.cmd == "autoLightOff") {
    if (context.pirCounter > 0) {
      Blynk.virtualWrite(V2, context.pirCounter);
      context.pirCounter = 0;
      sendStatus("Auto emergency light off!");
    } else {
      sendStatus("No movement detect!");
    }
  }

  if (context.cmd == "about") {
    SDCardManager::sendAboutInfo();
  }

  if (context.cmd == "almLocOn") {
    context.alarmCutOff = true;
    context.pcf.digitalWrite(context.alarmPin, LOW);
    sendStatus("Alarm Locked!");
  }

  if (context.cmd == "almLocOff") {
    context.alarmCutOff = false;
    sendStatus("Alarm Unlocked!");
  }

  if (context.cmd == "almOn") {
    if (context.alarmCutOff) {
      sendStatus("Alarm is Locked");
    } else {
      context.pcf.digitalWrite(context.alarmPin, HIGH);
      sendStatus("Alarm is on");
    }
  }

  if (context.cmd == "almOff") {
    context.pcf.digitalWrite(context.alarmPin, LOW);
    sendStatus("Alarm is off");
  }

  if (context.cmd == "pwrGon") {
    context.pcf.digitalWrite(6, LOW);
    delay(100);
    if (context.pcf.digitalRead(6) == LOW) {
      sendStatus("Lightning protection on");
    } else {
      sendStatus("Lightning protection failed!");
    }
  }

  if (context.cmd == "pwrGoff") {
    context.pcf.digitalWrite(6, HIGH);
    delay(100);
    if (context.pcf.digitalRead(6) == HIGH) {
      sendStatus("Lightning protection off");
    } else {
      sendStatus("Lightning protection failed!");
    }
  }

  if (context.cmd == "fan") {
    context.pcf.digitalWrite(context.fanPin, HIGH);
    delay(5000);
    context.pcf.digitalWrite(context.fanPin, LOW);
    sendStatus("Fan Checked!");
  }

  if (context.cmd == "reset") {
    sendStatus("Restarting ESP32...");
    delay(1000);
    esp_restart();
  }

  if (context.cmd == "rf1On") {
    context.pcf.digitalWrite(context.rf1Pin, LOW);
    sendStatus("RF SW1 ON");
  }
  if (context.cmd == "rf1Off") {
    context.pcf.digitalWrite(context.rf1Pin, HIGH);
    sendStatus("RF SW1 OFF");
  }

  if (context.cmd == "rf2On") {
    context.pcf.digitalWrite(context.rf2Pin, LOW);
    sendStatus("RF SW2 ON");
  }
  if (context.cmd == "rf2Off") {
    context.pcf.digitalWrite(context.rf2Pin, HIGH);
    sendStatus("RF SW2 OFF");
  }

  if (context.cmd == "rf3On") {
    context.pcf.digitalWrite(context.rf3Pin, LOW);
    sendStatus("RF SW3 ON");
  }
  if (context.cmd == "rf3Off") {
    context.pcf.digitalWrite(context.rf3Pin, HIGH);
    sendStatus("RF SW3 OFF");
  }

  if (context.cmd == "rf4On") {
    context.pcf.digitalWrite(context.rf4Pin, LOW);
    sendStatus("RF SW4 ON");
  }
  if (context.cmd == "rf4Off") {
    context.pcf.digitalWrite(context.rf4Pin, HIGH);
    sendStatus("RF SW4 OFF");
  }

  if (context.cmd == "rfCH1") {
    context.pcf.digitalWrite(context.rfChannelPin, HIGH);
    sendStatus("RF Channel 1");
  }

  if (context.cmd == "rfCH2") {
    context.pcf.digitalWrite(context.rfChannelPin, LOW);
    sendStatus("RF Channel 2");
  }

  if (context.cmd == "rfAllOff") {
    setRfAll(context.pcf, context.rf1Pin, context.rf2Pin, context.rf3Pin, context.rf4Pin, context.rfChannelPin, false);
  }

  if (context.cmd == "rfAllOn") {
    setRfAll(context.pcf, context.rf1Pin, context.rf2Pin, context.rf3Pin, context.rf4Pin, context.rfChannelPin, true);
  }

  if (context.cmd == "rfStatus") {
    sendRfStatus(context.pcf, context.rfChannelPin, context.rf1Pin, context.rf2Pin, context.rf3Pin, context.rf4Pin);
  }

  if (context.cmd == "Xon") {
    if (SDCardManager::getSetting("Xmode")) {
      sendStatus("Auto X mode is on!");
    }
    context.xModeEnabled = true;
    context.xModeFlag = true;
  }

  if (context.cmd == "Xoff") {
    context.xModeEnabled = false;
    context.xModeFlag = true;
  }

  if (context.cmd == "help") {
    sendHelp();
  }
}

}  // namespace CommandManager
