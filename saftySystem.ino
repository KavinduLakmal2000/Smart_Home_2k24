void saftySys() {  // checking all the temp sensors and voltage sensors if any problems send notification and cmd lines
  float avaTemp = envT + 2.5;

  if (systemTemp > avaTemp) {
    pcf1.digitalWrite(fan, HIGH);
  } else {
    pcf1.digitalWrite(fan, LOW);
  }

  avaTemp = 0;
  //////////////////////////////////////////////////////////////////////////////////////////
  if (VoltSensor > 265) {
    if (voltHighCount < 10) {
      voltHighCount++;
    }

    if (voltHighCount >= 8) {
      if (!sFlag1) {
        pcf1.digitalWrite(pwrCut, LOW);
        delay(1000);
        pcf1.digitalWrite(pwrCut, HIGH);
        delay(500);
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main 230V is too high!");
        sFlag1 = true;
      }
      Blynk.virtualWrite(V2, "Main 230V is too high! Count: " + String(voltHighCount));
      addError("Main 230V is too high!");
    }
  } else {
    if (sFlag1) {
      voltHighCount = 0;
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "Main 230V Back in Stable");
      sFlag1 = false;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if (VoltSensor > 90 && VoltSensor < 200) {

    if (voltLowCount < 10) {
      voltLowCount++;
    }

    if (voltLowCount >= 8) {
      if (!sFlag2) {
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main 230V is too low!");
        sFlag2 = true;
      }
      Blynk.virtualWrite(V2, "Main 230V is too Low! Count: " + String(voltLowCount));
      addError("Main 230V is too low!");
    }
  }

  else {
    if (sFlag2) {
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "Main 230V Back in Stable");
      voltLowCount = 0;
      sFlag2 = false;
    }
  }
  //////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_main > 17) {
    if (batteryMainHighCount < 10) {
      batteryMainHighCount++;
    }

    if (batteryMainHighCount >= 6) {
      if (!sFlag3) {
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main battery is overcharging!");
        LedAllOff();
        sFlag3 = true;
      }
      Blynk.virtualWrite(V2, "Main battery is overcharging! Count: " + String(batteryMainHighCount));
      addError("Main battery is overcharging!");
    }

  } else {
    if (sFlag3) {
      batteryMainHighCount = 0;
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "Main battery voltage is now stable");
      sFlag3 = false;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_main < 12) {
    if (batteryMainLowCount < 10) {
      batteryMainLowCount++;
    }

    if (batteryMainLowCount >= 5) {
      if (!sFlag4) {
        Blynk.logEvent("volts_amps_sensors", "Main battery voltage is too low!");
        LedAllOff();
        sFlag4 = true;
      }
      Blynk.virtualWrite(V2, "Main battery voltage is too low! Count: " + String(batteryMainLowCount));
      addError("Main battery voltage is too low!");
    }
  } else {
    if (sFlag4) {
      Blynk.logEvent("volts_amps_sensors", "Main battery voltage is now stable");
      batteryMainLowCount = 0;
      sFlag4 = false;
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_sys > 6) {
    if (batterySysHighCount < 10) {
      batterySysHighCount++;
    }
    if (batterySysHighCount >= 5) {
      if (!sFlag5) {
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "System voltage is too high!");
        LedAllOff();
        sFlag5 = true;
      }
      Blynk.virtualWrite(V2, "System voltage is too high! Count: " + String(batterySysHighCount));
      addError("System voltage is too high!");
    }
  } else {
    if (sFlag5) {
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "System voltage now stable");
      batterySysHighCount = 0;
      sFlag5 = false;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_sys < 3.2) {
    if (batterySysLowCount < 10) {
      batterySysLowCount++;
    }
    if (batterySysLowCount >= 5) {
      if (!sFlag6) {
        Blynk.logEvent("volts_amps_sensors", "System voltage is too low!");
        sFlag6 = true;
      }
      Blynk.virtualWrite(V2, "System voltage is too low! Count: " + String(batterySysLowCount));
      addError("System voltage is too low!");
    }
  } else {
    if (sFlag6) {
      Blynk.logEvent("volts_amps_sensors", "System voltage is now stable");
      batterySysLowCount = 0;
      sFlag6 = false;
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////

  if (systemTemp > 39) {
    if (systemTempCount < 10) {
      systemTempCount++;
    }
    if (systemTempCount >= 5) {
      if (!sFlag7) {
        Blynk.logEvent("temp", "System Temp is too high!");
        LedAllOff();
        sFlag7 = true;
      }
      Blynk.virtualWrite(V2, "System Temp is too high! Count: " + String(systemTempCount));
      addError("System Temp is too high!");
    }
  } else {
    if (sFlag7) {
      Blynk.logEvent("temp", "System Temp is back in normal");
      systemTempCount = 0;
      sFlag7 = false;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////

  if (batteryTemp > 36) {
    if (batteryTempCount < 10) {
      batteryTempCount++;
    }
    if (batteryTempCount >= 5) {
      if (!sFlag8) {
        Blynk.logEvent("temp", "Battery Temp is too high!");
        LedAllOff();
        sFlag8 = true;
      }
      Blynk.virtualWrite(V2, "Battery Temp is too high! Count: " + String(batteryTempCount));
      addError("Battery Temp is too high!");
    }
  } else {
    if (sFlag7) {
      Blynk.logEvent("temp", "Battery Temp back in normal");
      batteryTempCount = 0;
      sFlag7 = false;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////

  if (envT > 37) {
    if (envTCount < 10) {
      envTCount++;
    }
    if (envTCount >= 6) {
      if (!sFlag9) {
        Blynk.logEvent("temp", "Power Supply Temp is too high!");
        LedAllOff();
        sFlag9 = true;
      }
      Blynk.virtualWrite(V2, "Power Supply Temp is too high! Count: " + String(envTCount));
      addError("Power Supply Temp is too high!");
    }
  } else {
    if (sFlag9) {
      Blynk.logEvent("temp", "Power Supply Temp is back in normal");
      envTCount = 0;
      sFlag9 = false;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////
  if (systemTemp > 40 || batteryTemp > 45 || envT > 40) {
    if (fireDetectionCount < 20) {
      fireDetectionCount++;
    }
    if (fireDetectionCount >= 18) {
      if (!sFlag10) {
        Blynk.logEvent("temp", "🔥 Fire Detected!!!");
        pcf1.digitalWrite(pwrCut, LOW);
        delay(1000);
        pcf1.digitalWrite(pwrCut, HIGH);
        LedAllOff();
        sFlag10 = true;
      }
      Blynk.virtualWrite(V2, "🔥 Fire Detected!!! Count: " + String(fireDetectionCount));
      addError("Fire Detected!!!");
    }
  } else {
    if (sFlag10) {
      fireDetectionCount = 0;
      sFlag10 = false;
    }
  }
  ////////////////////////////////////////////////Lightning protect on/////////////////////////////////////////////////////

  //lightnin Protect
  if (rainSensor < rainThreshold) {
    rainDetectCount++;
    if (rainDetectCount > 10) {
      rainCounter = 300;
      Blynk.logEvent("env", "Rain Detected!!");
    }
  }

  else {
    rainDetectCount = 0;  // Reset if no rain detected
  }

  if (rainCounter > 0) {
    pcf1.digitalWrite(lightninProtect, LOW);
    Blynk.virtualWrite(V2, "Lightning protect on");
    logRain = String(timestamp) + ", Raining";
    rainProtect = true;
    rainCounter--;
  }

  else {
    if (rainProtect) {
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.virtualWrite(V2, "Lightning protect off");
      logRain = String(timestamp) + ", No Rain";
      rainProtect = false;
    }
  }
}