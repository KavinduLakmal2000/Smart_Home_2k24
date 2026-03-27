#ifndef SD_CARD_MANAGER_H
#define SD_CARD_MANAGER_H

#include <Arduino.h>
#include <SPI.h>

namespace SDCardManager {

struct SensorLogData {
  const char* timestamp;
  float voltSensor;
  float ampSensor;
  float batteryVoltageMain;
  float batteryVoltageSys;
  float systemTemp;
  float batteryTemp;
  float envT;
  const String& rainLog;
};

using StatusCallback = void (*)(const String&);
using EventCallback = void (*)(const char*, const char*);

void setCallbacks(StatusCallback statusCallback, EventCallback eventCallback = nullptr);
bool begin(SPIClass& spi, int sck, int miso, int mosi, int cs);

void logData(const SensorLogData& data, String& logMainVoltStatus);
void addError(String& errorBuffer, const char* timestamp, const String& message);
void flushErrors(String& errorBuffer, const char* timestamp);
void sendLastLinesFromFile(const char* filename, const String& label, int maxLines = 30);
void checkCardSpace();

void updateAboutFile(const char* softVer);
void sendAboutInfo();
void logFirmwareVersion(const char* timestamp, const char* softVer, const char* whatsNew);
void sendFirmwareLog();

bool setSetting(const char* key, int value);
int getSetting(const char* key);
void sendSettings();

}  // namespace SDCardManager

#endif
