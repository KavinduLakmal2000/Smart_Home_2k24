#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>

namespace DisplayManager {

struct DisplayData {
  float voltSensor;
  float batteryVoltageSys;
  float batteryVoltageMain;
  float ampSensor;
  float systemTemp;
  float batteryTemp;
  float envT;
  float envH;
};

void drawDisplay1(Adafruit_SSD1306& display, const DisplayData& data);
void drawDisplay2(Adafruit_SSD1306& display, const DisplayData& data);
void drawDisplay3(Adafruit_SSD1306& display, const DisplayData& data, int screenWidth, int screenHeight);
void drawDisplay4(Adafruit_SSD1306& display, int screenWidth, int screenHeight);

}  // namespace DisplayManager

#endif
