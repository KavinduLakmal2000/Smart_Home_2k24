#include "SafetySystemManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>

namespace SafetySystemManager {

void run(SafetySystemContext& context) {
  float avaTemp = context.envT + 2.5f;

  if (context.systemTemp > avaTemp) {
    context.pcf.digitalWrite(context.fanPin, HIGH);
  } else {
    context.pcf.digitalWrite(context.fanPin, LOW);
  }

  if (context.voltSensor > 265) {
    if (context.voltHighCount < 10) {
      context.voltHighCount++;
    }

    if (context.voltHighCount >= 8) {
      if (!context.sFlag1) {
        context.pcf.digitalWrite(context.powerCutPin, LOW);
        delay(1000);
        context.pcf.digitalWrite(context.powerCutPin, HIGH);
        delay(500);
        context.pcf.digitalWrite(context.lightningProtectPin, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main 230V is too high!");
        context.sFlag1 = true;
      }
      Blynk.virtualWrite(V2, "Main 230V is too high! Count: " + String(context.voltHighCount));
      context.addError("Main 230V is too high!");
    }
  } else if (context.sFlag1) {
    context.voltHighCount = 0;
    context.pcf.digitalWrite(context.lightningProtectPin, HIGH);
    Blynk.logEvent("volts_amps_sensors", "Main 230V Back in Stable");
    context.sFlag1 = false;
  }

  if (context.voltSensor > 90 && context.voltSensor < 200) {
    if (context.voltLowCount < 10) {
      context.voltLowCount++;
    }

    if (context.voltLowCount >= 8) {
      if (!context.sFlag2) {
        context.pcf.digitalWrite(context.lightningProtectPin, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main 230V is too low!");
        context.sFlag2 = true;
      }
      Blynk.virtualWrite(V2, "Main 230V is too Low! Count: " + String(context.voltLowCount));
      context.addError("Main 230V is too low!");
    }
  } else if (context.sFlag2) {
    context.pcf.digitalWrite(context.lightningProtectPin, HIGH);
    Blynk.logEvent("volts_amps_sensors", "Main 230V Back in Stable");
    context.voltLowCount = 0;
    context.sFlag2 = false;
  }

  if (context.batteryVoltageMain > 17) {
    if (context.batteryMainHighCount < 10) {
      context.batteryMainHighCount++;
    }

    if (context.batteryMainHighCount >= 6) {
      if (!context.sFlag3) {
        context.pcf.digitalWrite(context.lightningProtectPin, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main battery is overcharging!");
        context.ledAllOff();
        context.sFlag3 = true;
      }
      Blynk.virtualWrite(V2, "Main battery is overcharging! Count: " + String(context.batteryMainHighCount));
      context.addError("Main battery is overcharging!");
    }
  } else if (context.sFlag3) {
    context.batteryMainHighCount = 0;
    context.pcf.digitalWrite(context.lightningProtectPin, HIGH);
    Blynk.logEvent("volts_amps_sensors", "Main battery voltage is now stable");
    context.sFlag3 = false;
  }

  if (context.batteryVoltageMain < 12) {
    if (context.batteryMainLowCount < 10) {
      context.batteryMainLowCount++;
    }

    if (context.batteryMainLowCount >= 5) {
      if (!context.sFlag4) {
        Blynk.logEvent("volts_amps_sensors", "Main battery voltage is too low!");
        context.ledAllOff();
        context.sFlag4 = true;
      }
      Blynk.virtualWrite(V2, "Main battery voltage is too low! Count: " + String(context.batteryMainLowCount));
      context.addError("Main battery voltage is too low!");
    }
  } else if (context.sFlag4) {
    Blynk.logEvent("volts_amps_sensors", "Main battery voltage is now stable");
    context.batteryMainLowCount = 0;
    context.sFlag4 = false;
  }

  if (context.batteryVoltageSys > 6) {
    if (context.batterySysHighCount < 10) {
      context.batterySysHighCount++;
    }
    if (context.batterySysHighCount >= 5) {
      if (!context.sFlag5) {
        context.pcf.digitalWrite(context.lightningProtectPin, LOW);
        Blynk.logEvent("volts_amps_sensors", "System voltage is too high!");
        context.ledAllOff();
        context.sFlag5 = true;
      }
      Blynk.virtualWrite(V2, "System voltage is too high! Count: " + String(context.batterySysHighCount));
      context.addError("System voltage is too high!");
    }
  } else if (context.sFlag5) {
    context.pcf.digitalWrite(context.lightningProtectPin, HIGH);
    Blynk.logEvent("volts_amps_sensors", "System voltage now stable");
    context.batterySysHighCount = 0;
    context.sFlag5 = false;
  }

  if (context.batteryVoltageSys < 3.2) {
    if (context.batterySysLowCount < 10) {
      context.batterySysLowCount++;
    }
    if (context.batterySysLowCount >= 5) {
      if (!context.sFlag6) {
        Blynk.logEvent("volts_amps_sensors", "System voltage is too low!");
        context.sFlag6 = true;
      }
      Blynk.virtualWrite(V2, "System voltage is too low! Count: " + String(context.batterySysLowCount));
      context.addError("System voltage is too low!");
    }
  } else if (context.sFlag6) {
    Blynk.logEvent("volts_amps_sensors", "System voltage is now stable");
    context.batterySysLowCount = 0;
    context.sFlag6 = false;
  }

  if (context.systemTemp > 39) {
    if (context.systemTempCount < 10) {
      context.systemTempCount++;
    }
    if (context.systemTempCount >= 5) {
      if (!context.sFlag7) {
        Blynk.logEvent("temp", "System Temp is too high!");
        context.ledAllOff();
        context.sFlag7 = true;
      }
      Blynk.virtualWrite(V2, "System Temp is too high! Count: " + String(context.systemTempCount));
      context.addError("System Temp is too high!");
    }
  } else if (context.sFlag7) {
    Blynk.logEvent("temp", "System Temp is back in normal");
    context.systemTempCount = 0;
    context.sFlag7 = false;
  }

  if (context.batteryTemp > 36) {
    if (context.batteryTempCount < 10) {
      context.batteryTempCount++;
    }
    if (context.batteryTempCount >= 5) {
      if (!context.sFlag8) {
        Blynk.logEvent("temp", "Battery Temp is too high!");
        context.ledAllOff();
        context.sFlag8 = true;
      }
      Blynk.virtualWrite(V2, "Battery Temp is too high! Count: " + String(context.batteryTempCount));
      context.addError("Battery Temp is too high!");
    }
  } else if (context.sFlag7) {
    Blynk.logEvent("temp", "Battery Temp back in normal");
    context.batteryTempCount = 0;
    context.sFlag7 = false;
  }

  if (context.envT > 37) {
    if (context.envTCount < 10) {
      context.envTCount++;
    }
    if (context.envTCount >= 6) {
      if (!context.sFlag9) {
        Blynk.logEvent("temp", "Power Supply Temp is too high!");
        context.ledAllOff();
        context.sFlag9 = true;
      }
      Blynk.virtualWrite(V2, "Power Supply Temp is too high! Count: " + String(context.envTCount));
      context.addError("Power Supply Temp is too high!");
    }
  } else if (context.sFlag9) {
    Blynk.logEvent("temp", "Power Supply Temp is back in normal");
    context.envTCount = 0;
    context.sFlag9 = false;
  }

  if (context.systemTemp > 40 || context.batteryTemp > 45 || context.envT > 40) {
    if (context.fireDetectionCount < 20) {
      context.fireDetectionCount++;
    }
    if (context.fireDetectionCount >= 18) {
      if (!context.sFlag10) {
        Blynk.logEvent("temp", "🔥 Fire Detected!!!");
        context.pcf.digitalWrite(context.powerCutPin, LOW);
        delay(1000);
        context.pcf.digitalWrite(context.powerCutPin, HIGH);
        context.ledAllOff();
        context.sFlag10 = true;
      }
      Blynk.virtualWrite(V2, "🔥 Fire Detected!!! Count: " + String(context.fireDetectionCount));
      context.addError("Fire Detected!!!");
    }
  } else if (context.sFlag10) {
    context.fireDetectionCount = 0;
    context.sFlag10 = false;
  }

  if (context.rainSensor < 1018) {
    context.rainDetectCount++;
    if (context.rainDetectCount > 10) {
      context.rainCounter = 300;
      Blynk.logEvent("env", "Rain Detected!!");
    }
  } else {
    context.rainDetectCount = 0;
  }

  if (context.rainCounter > 0) {
    context.pcf.digitalWrite(context.lightningProtectPin, LOW);
    Blynk.virtualWrite(V2, "Lightning protect on");
    context.logRain = String(context.timestamp) + ", Raining";
    context.rainProtect = true;
    context.rainCounter--;
  } else if (context.rainProtect) {
    context.pcf.digitalWrite(context.lightningProtectPin, HIGH);
    Blynk.virtualWrite(V2, "Lightning protect off");
    context.logRain = String(context.timestamp) + ", No Rain";
    context.rainProtect = false;
  }
}

}  // namespace SafetySystemManager
