//----------------------------------------------------------------------------- SD Card START----------------------------------------------------------------------------------------
void DataLog() {
  /////////////////////////////////////////////////////////////// log ac 230v ////////////////////////////////////////////
  String logMainVolt = String(timestamp) + ", " + String(VoltSensor);
  File ACVolt = SD.open("/AC230v_log.csv", FILE_APPEND);
  if (ACVolt) {
    ACVolt.println(logMainVolt);
    ACVolt.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Ac Volt");
  }

  //////////////////////////////////////////////////////////////// log AC 230 on or off ///////////////////////////////

  if (VoltSensor > 210) {
    logMainVoltStatus = String(timestamp) + ", AC ON";
  } else {
    logMainVoltStatus = String(timestamp) + ", AC OFF";
  }

  File ACVoltSts = SD.open("/AC230v_Status_log.csv", FILE_APPEND);
  if (ACVoltSts) {
    ACVoltSts.println(logMainVoltStatus);
    ACVoltSts.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Ac Volt Status");
  }

  //////////////////////////////////////////////log amp sensor/////////////////////////////////////////////////////

  String logAmpSen = String(timestamp) + ", " + String(AmpSensor);
  File amps = SD.open("/Amp_Sensor_log.csv", FILE_APPEND);
  if (amps) {
    amps.println(logAmpSen);
    amps.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Amp sensor");
  }


  //////////////////////////////////////////////// main battery /////////////////////////////////////////////////////
  String logMainBattery = String(timestamp) + ", " + String(batteryVoltage_main);
  File MainBat = SD.open("/Main_battery_log.csv", FILE_APPEND);
  if (MainBat) {
    MainBat.println(logMainBattery);
    MainBat.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Main Battery");
  }


  //////////////////////////////////////////////// System Battery //////////////////////////////////////////////////
  String logSysBattery = String(timestamp) + ", " + String(batteryVoltage_sys);
  File SysBat = SD.open("/System_battery_log.csv", FILE_APPEND);
  if (SysBat) {
    SysBat.println(logSysBattery);
    SysBat.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log System Battery");
  }


  ////////////////////////////////////////////////// system temp sensor value///////////////////////////////////////////
  String logSysTemp = String(timestamp) + ", " + String(systemTemp);
  File SysTemplog = SD.open("/System_temp_log.csv", FILE_APPEND);
  if (SysTemplog) {
    SysTemplog.println(logSysTemp);
    SysTemplog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log System temp");
  }

  ////////////////////////////////////////////////////// main battery temp value ///////////////////////////////////////////
  String logMainBatTemp = String(timestamp) + ", " + String(batteryTemp);
  File MainBatTemplog = SD.open("/Battery_temp_log.csv", FILE_APPEND);
  if (MainBatTemplog) {
    MainBatTemplog.println(logMainBatTemp);
    MainBatTemplog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Main Battery temp");
  }

  //////////////////////////////////////////////////////// environment temp value //////////////////////////////////////////
  String logEnvTemp = String(timestamp) + ", " + String(envT);
  File EnvTemplog = SD.open("/Environment_temp_log.csv", FILE_APPEND);
  if (EnvTemplog) {
    EnvTemplog.println(logEnvTemp);
    EnvTemplog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Environment temp");
  }

  /////////////////////////////////////////////////////////// Rain Log ////////////////////////////////////////////////////
  File RainLog = SD.open("/Rain_log.csv", FILE_APPEND);
  if (RainLog) {
    RainLog.println(logRain);
    RainLog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Environment Rain");
  }
}

//////////////////////////////////////////////////////////// safety system log ////////////////////////////////

void addError(String message) {
  errorBuffer += String(timestamp) + ", " + message + "\n";
}

void flushErrors() {
  if (errorBuffer.length() > 0) {
    // Write all collected errors
    File SafetyLog = SD.open("/Safety_system_Error_log.csv", FILE_APPEND);
    if (SafetyLog) {
      SafetyLog.print(errorBuffer);
      SafetyLog.close();
    } else {
      Blynk.virtualWrite(V2, "Failed to log Safety system error");
    }
    errorBuffer = "";
  } else {
    String logEntry = String(timestamp) + ", All good";
    File SafetyLog = SD.open("/Safety_system_Status_log.csv", FILE_WRITE);
    if (SafetyLog) {
      SafetyLog.println(logEntry);
      SafetyLog.close();
    } else {
      Blynk.virtualWrite(V2, "Failed to log Safety system status");
    }
  }
}
//------------------------------------------------------ check safety log in blynk cmd ----------------------------------

void sendLastLinesFromFile(const char* filename, String label) {
  File logFile = SD.open(filename, FILE_READ);
  if (!logFile) {
    Blynk.virtualWrite(V2, "Failed to open " + String(filename));
    return;
  }

  String lastLines[MAX_LINES];
  int count = 0;
  String currentLine = "";

  while (logFile.available()) {
    char c = logFile.read();
    if (c == '\n') {
      lastLines[count % MAX_LINES] = currentLine;
      count++;
      currentLine = "";
    } else {
      currentLine += c;
    }
  }

  if (currentLine.length() > 0) {
    lastLines[count % MAX_LINES] = currentLine;
    count++;
  }

  logFile.close();

  int start = (count > MAX_LINES) ? (count - MAX_LINES) : 0;

  Blynk.virtualWrite(V2, "---- " + label + " ----");
  for (int i = start; i < count; i++) {
    Blynk.virtualWrite(V2, lastLines[i % MAX_LINES]);
    delay(50);
  }
}
// --------------------------------------------------- Check SD Card Space ------------------------------------------
unsigned long getDirSize(File dir) {
  unsigned long total = 0;
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    if (entry.isDirectory()) {
      total += getDirSize(entry);
    } else {
      total += entry.size();
    }
    entry.close();
  }
  return total;
}

void checkCardSpace() {
  File root = SD.open("/");
  unsigned long totalSize = getDirSize(root);
  root.close();
  unsigned long totalSizeMB = totalSize / (1024 * 1024);

  // 1.5 GB = 1.5 * 1024 * 1024 * 1024
  if (totalSizeMB > 1536) {
    Blynk.virtualWrite(V2, "⚠️ WARNING: SD card almost full!");
    Blynk.logEvent("volts_amps_sensors", "WARNING: SD card almost full!");
  } else {
    Blynk.virtualWrite(V2, "SD card space OK");
    Blynk.virtualWrite(V2, String(totalSizeMB) + " MB");
  }
}
//---------------------------------------------------------------------- add about.text -------------------------------------------------
// Function to create/update about.txt
void updateAboutFile() {
  File file = SD.open("/about.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open about.txt for writing");
    return;
  }
  file.println(SoftVer);
  file.println("Created BY KLTECHNOLOGY");
  file.println("© 2024 Kavindu Lakmal. All rights reserved");
  file.close();
}

// Function to read and send file via Blynk
void sendAboutInfo() {
  File file = SD.open("/about.txt", FILE_READ);
  if (!file) {
    Blynk.virtualWrite(V2, "⚠️ No about.txt found");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      Blynk.virtualWrite(V2, line);
      delay(150);
    }
  }

  file.close();
}
//-------------------------------------------------------------------------- log firmware data --------------------------------------------------
void logFirmwareVersion() {
  // First check if file already contains this version
  timeDateUpdate();
  delay(1000);
  File file = SD.open("/What_new_log.csv", FILE_READ);
  if (file) {
    String content = "";
    while (file.available()) {
      content = file.readStringUntil('\n');
      if (content.indexOf(SoftVer) != -1) {
        Blynk.virtualWrite(V2, "Firmware is up to date");
        file.close();
        return;
      }
    }
    file.close();
  }

  // If not found → append new version entry
  File logFile = SD.open("/What_new_log.csv", FILE_APPEND);
  if (logFile) {
    String entry = String(timestamp) + ", " + SoftVer + ", " + whatsNew;
    logFile.println(entry);
    Blynk.virtualWrite(V2, "Firmware Updated To New Version");
    logFile.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to write changelog");
  }
}

void sendFirmwareLog() {
  File logFile = SD.open("/What_new_log.csv", FILE_READ);
  if (!logFile) {
    Blynk.virtualWrite(V2, "⚠️ No firmware log found");
    return;
  }

  while (logFile.available()) {
    String line = logFile.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      Blynk.virtualWrite(V2, line);
      delay(120);  // prevent flooding Blynk
    }
  }

  logFile.close();
}

bool SetSetting(const char* key, int value) {  //---------------------------- sttings save
  String newValue;
  // Store "rain" as a quoted number, others as HIGH/LOW
  if (String(key) == "rain") {
    newValue = "\"" + String(value) + "\"";
  } else {
    newValue = (value == HIGH) ? "HIGH" : "LOW";
  }

  String content = "";
  bool found = false;

  File file = SD.open("/settings.text", FILE_READ);
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      line.trim();
      if (line.length() == 0) continue;

      if (line.startsWith(key)) {
        content += String(key) + " = " + newValue + "\n";
        found = true;
      } else {
        content += line + "\n";
      }
    }
    file.close();
  }

  if (!found) {
    content += String(key) + " = " + newValue + "\n";
  }

  File writeFile = SD.open("/settings.text", FILE_WRITE);
  if (!writeFile) return false;
  writeFile.print(content);
  writeFile.close();

  return true;
}

int GetSettings(const char* key) {  // ----------------------------------- Read settings
  File file = SD.open("/settings.text", FILE_READ);
  if (!file) return LOW;  // Default

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.startsWith(key)) {
      int index = line.indexOf('=');
      if (index > 0) {
        String val = line.substring(index + 1);
        val.trim();

        // Strip quotes if they exist (e.g. "1023" -> 1023)
        if (val.startsWith("\"") && val.endsWith("\"")) {
          val = val.substring(1, val.length() - 1);
        }

        file.close();
        if (val == "HIGH") return HIGH;
        if (val == "LOW") return LOW;
        return val.toInt(); // Return the numeric threshold
      }
    }
  }

  file.close();
  return LOW;  // default if not found
}

void SendSettingsToCmd() {  // ------------------------------------------------- Read all the settings in once
  File file = SD.open("/settings.text", FILE_READ);

  if (!file) {
    Blynk.virtualWrite(V2, "⚠ No settings.text found");
    return;
  }

  Blynk.virtualWrite(V2, "📌 Current Settings:");

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.length() == 0) continue;

    int idx = line.indexOf('=');
    if (idx > 0) {
      String key = line.substring(0, idx);
      String value = line.substring(idx + 1);

      Blynk.virtualWrite(V2, key + " = " + value);
      delay(150);
    }
  }

  file.close();
}


//----------------------------------------------------------------------------- SD Card END ----------------------------------------------------------------------------------------
