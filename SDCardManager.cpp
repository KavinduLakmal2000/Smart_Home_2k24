#include "SDCardManager.h"

#include <SD.h>

namespace SDCardManager {

namespace {
StatusCallback gStatusCallback = nullptr;
EventCallback gEventCallback = nullptr;

void notify(const String& message) {
  if (gStatusCallback) {
    gStatusCallback(message);
  }
}

void notifyEvent(const char* eventName, const char* message) {
  if (gEventCallback) {
    gEventCallback(eventName, message);
  }
}

bool appendLine(const char* path, const String& line, const String& errorMessage) {
  File file = SD.open(path, FILE_APPEND);
  if (!file) {
    notify(errorMessage);
    return false;
  }

  file.println(line);
  file.close();
  return true;
}

unsigned long getDirSize(File dir) {
  unsigned long total = 0;

  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }

    if (entry.isDirectory()) {
      total += getDirSize(entry);
    } else {
      total += entry.size();
    }
    entry.close();
  }

  return total;
}
}  // namespace

void setCallbacks(StatusCallback statusCallback, EventCallback eventCallback) {
  gStatusCallback = statusCallback;
  gEventCallback = eventCallback;
}

bool begin(SPIClass& spi, int sck, int miso, int mosi, int cs) {
  spi.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs, spi)) {
    notify("Card Mount Failed");
    return false;
  }

  notify("SD card initialized successfully!");
  return true;
}

void logData(const SensorLogData& data, String& logMainVoltStatus) {
  appendLine("/AC230v_log.csv",
             String(data.timestamp) + ", " + String(data.voltSensor),
             "Failed to log Ac Volt");

  if (data.voltSensor > 210) {
    logMainVoltStatus = String(data.timestamp) + ", AC ON";
  } else {
    logMainVoltStatus = String(data.timestamp) + ", AC OFF";
  }
  appendLine("/AC230v_Status_log.csv", logMainVoltStatus, "Failed to log Ac Volt Status");

  appendLine("/Amp_Sensor_log.csv",
             String(data.timestamp) + ", " + String(data.ampSensor),
             "Failed to log Amp sensor");

  appendLine("/Main_battery_log.csv",
             String(data.timestamp) + ", " + String(data.batteryVoltageMain),
             "Failed to log Main Battery");

  appendLine("/System_battery_log.csv",
             String(data.timestamp) + ", " + String(data.batteryVoltageSys),
             "Failed to log System Battery");

  appendLine("/System_temp_log.csv",
             String(data.timestamp) + ", " + String(data.systemTemp),
             "Failed to log System temp");

  appendLine("/Battery_temp_log.csv",
             String(data.timestamp) + ", " + String(data.batteryTemp),
             "Failed to log Main Battery temp");

  appendLine("/Environment_temp_log.csv",
             String(data.timestamp) + ", " + String(data.envT),
             "Failed to log Environment temp");

  appendLine("/Rain_log.csv", data.rainLog, "Failed to log Environment Rain");
}

void addError(String& errorBuffer, const char* timestamp, const String& message) {
  errorBuffer += String(timestamp) + ", " + message + "\n";
}

void flushErrors(String& errorBuffer, const char* timestamp) {
  if (errorBuffer.length() > 0) {
    File safetyLog = SD.open("/Safety_system_Error_log.csv", FILE_APPEND);
    if (safetyLog) {
      safetyLog.print(errorBuffer);
      safetyLog.close();
    } else {
      notify("Failed to log Safety system error");
    }
    errorBuffer = "";
    return;
  }

  File safetyLog = SD.open("/Safety_system_Status_log.csv", FILE_WRITE);
  if (safetyLog) {
    safetyLog.println(String(timestamp) + ", All good");
    safetyLog.close();
  } else {
    notify("Failed to log Safety system status");
  }
}

void sendLastLinesFromFile(const char* filename, const String& label, int maxLines) {
  File logFile = SD.open(filename, FILE_READ);
  if (!logFile) {
    notify("Failed to open " + String(filename));
    return;
  }

  String* lastLines = new String[maxLines];
  int count = 0;
  String currentLine = "";

  while (logFile.available()) {
    char c = logFile.read();
    if (c == '\n') {
      lastLines[count % maxLines] = currentLine;
      count++;
      currentLine = "";
    } else {
      currentLine += c;
    }
  }

  if (currentLine.length() > 0) {
    lastLines[count % maxLines] = currentLine;
    count++;
  }

  logFile.close();

  int start = (count > maxLines) ? (count - maxLines) : 0;
  notify("---- " + label + " ----");
  for (int i = start; i < count; i++) {
    notify(lastLines[i % maxLines]);
    delay(50);
  }

  delete[] lastLines;
}

void checkCardSpace() {
  File root = SD.open("/");
  if (!root) {
    notify("Failed to open SD root");
    return;
  }

  unsigned long totalSize = getDirSize(root);
  root.close();
  unsigned long totalSizeMB = totalSize / (1024 * 1024);

  if (totalSizeMB > 1536) {
    notify("WARNING: SD card almost full!");
    notifyEvent("volts_amps_sensors", "WARNING: SD card almost full!");
  } else {
    notify("SD card space OK");
    notify(String(totalSizeMB) + " MB");
  }
}

void updateAboutFile(const char* softVer) {
  File file = SD.open("/about.txt", FILE_WRITE);
  if (!file) {
    notify("Failed to open about.txt for writing");
    return;
  }

  file.println(softVer);
  file.println("Created BY KLTECHNOLOGY");
  file.println("(c) 2024 Kavindu Lakmal. All rights reserved");
  file.close();
}

void sendAboutInfo() {
  File file = SD.open("/about.txt", FILE_READ);
  if (!file) {
    notify("No about.txt found");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      notify(line);
      delay(150);
    }
  }

  file.close();
}

void logFirmwareVersion(const char* timestamp, const char* softVer, const char* whatsNew) {
  File file = SD.open("/What_new_log.csv", FILE_READ);
  if (file) {
    while (file.available()) {
      String content = file.readStringUntil('\n');
      if (content.indexOf(softVer) != -1) {
        notify("Firmware is up to date");
        file.close();
        return;
      }
    }
    file.close();
  }

  File logFile = SD.open("/What_new_log.csv", FILE_APPEND);
  if (!logFile) {
    notify("Failed to write changelog");
    return;
  }

  String entry = String(timestamp) + ", " + softVer + ", " + whatsNew;
  logFile.println(entry);
  logFile.close();
  notify("Firmware Updated To New Version");
}

void sendFirmwareLog() {
  File logFile = SD.open("/What_new_log.csv", FILE_READ);
  if (!logFile) {
    notify("No firmware log found");
    return;
  }

  while (logFile.available()) {
    String line = logFile.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      notify(line);
      delay(120);
    }
  }

  logFile.close();
}

bool setSetting(const char* key, int value) {
  String newValue = (value == HIGH) ? "HIGH" : "LOW";
  String content = "";
  bool found = false;

  File file = SD.open("/settings.txt", FILE_READ);
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      line.trim();

      if (line.startsWith(key)) {
        content += String(key) + "=" + newValue + "\n";
        found = true;
      } else {
        content += line + "\n";
      }
    }
    file.close();
  }

  if (!found) {
    content += String(key) + "=" + newValue + "\n";
  }

  File writeFile = SD.open("/settings.txt", FILE_WRITE);
  if (!writeFile) {
    return false;
  }

  writeFile.print(content);
  writeFile.close();
  return true;
}

int getSetting(const char* key) {
  File file = SD.open("/settings.txt", FILE_READ);
  if (!file) {
    return LOW;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.startsWith(key)) {
      int index = line.indexOf('=');
      if (index > 0) {
        String value = line.substring(index + 1);
        value.trim();
        file.close();
        return (value == "HIGH") ? HIGH : LOW;
      }
    }
  }

  file.close();
  return LOW;
}

void sendSettings() {
  File file = SD.open("/settings.txt", FILE_READ);
  if (!file) {
    notify("No settings.txt found");
    return;
  }

  notify("Current Settings:");
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) {
      continue;
    }

    int idx = line.indexOf('=');
    if (idx > 0) {
      String key = line.substring(0, idx);
      String value = line.substring(idx + 1);
      notify(key + " = " + value);
      delay(150);
    }
  }

  file.close();
}

}  // namespace SDCardManager
