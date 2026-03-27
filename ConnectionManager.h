#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>

namespace ConnectionManager {

struct ConnectionContext {
  Adafruit_SSD1306& display;
  Adafruit_NeoPixel& rgbLed;
  char* ssid;
  char* pass;
  int& wifiRetryCount;
  int& blynkRetryCount;
  unsigned long& lastWiFiRetry;
  unsigned long& lastBlynkRetry;
  unsigned long retryInterval;
  float batteryVoltageMain;
  float voltSensor;
  float envT;
  unsigned long resetCounter;
  int screenWidth;
  int screenHeight;
  void (*renderConnectedDisplay)();
  void (*timeDateUpdate)();
};

void run(ConnectionContext& context);

}  // namespace ConnectionManager

#endif
