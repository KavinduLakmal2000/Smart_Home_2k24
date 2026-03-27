#include "DisplayManager.h"

namespace DisplayManager {

namespace {
float gAngle = 0.0f;
const float kSpeed = 0.05f;
}

void drawDisplay1(Adafruit_SSD1306& display, const DisplayData& data) {
  display.setCursor(0, 20);
  display.print(F("System Temp: "));
  display.setCursor(80, 20);
  display.print(data.systemTemp);
  display.print(F(" C"));

  display.setCursor(0, 30);
  display.print(F("Battery Temp: "));
  display.setCursor(80, 30);
  display.print(data.batteryTemp);
  display.print(F(" C"));

  display.setCursor(0, 40);
  display.print(F("Env Temp: "));
  display.setCursor(80, 40);
  display.print(data.envT);
  display.print(F(" C"));

  display.setCursor(0, 50);
  display.print(F("Env Humidity: "));
  display.setCursor(80, 50);
  display.print(data.envH);
  display.print(F(" C"));
}

void drawDisplay2(Adafruit_SSD1306& display, const DisplayData& data) {
  display.setCursor(0, 20);
  display.print(F("Main Volt:"));
  display.setCursor(80, 20);
  display.print(data.voltSensor);
  display.setCursor(112, 20);
  display.print(F(" v"));

  display.setCursor(0, 30);
  display.print(F("System Volt:"));
  display.setCursor(80, 30);
  display.print(data.batteryVoltageSys);
  display.setCursor(112, 30);
  display.print(F(" v"));

  display.setCursor(0, 40);
  display.print(F("Battery Volt:"));
  display.setCursor(80, 40);
  display.print(data.batteryVoltageMain);
  display.setCursor(112, 40);
  display.print(F(" v"));

  display.setCursor(0, 50);
  display.print(F("Amps:"));
  display.setCursor(80, 50);
  display.print(data.ampSensor);
  display.setCursor(112, 50);
  display.print(F(" A"));
}

void drawDisplay3(Adafruit_SSD1306& display, const DisplayData& data, int screenWidth, int screenHeight) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 0);
  display.print(F("- Smart House -"));
  display.setCursor(0, 0);
  display.print(F("K"));
  display.setCursor(43, 9);
  display.printf("Power Out");
  display.setCursor(0, 9);
  display.print(F("L"));
  display.setCursor(122, 0);
  display.print(F("2"));
  display.setCursor(122, 9);
  display.print(F("4"));
  display.drawLine(0, screenHeight / 3.6, screenWidth, screenHeight / 3.6, SSD1306_WHITE);

  drawDisplay2(display, data);
}

void drawDisplay4(Adafruit_SSD1306& display, int screenWidth, int screenHeight) {
  int cx = screenWidth / 2;
  int cy = (screenHeight / 2) + 20;

  const char* logo = "KL";
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  float radius = 10;
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(logo, 0, 0, &x1, &y1, &w, &h);

  int x = cx - (w / 2) + cos(gAngle) * radius;
  int y = cy - (h / 2) + sin(gAngle) * radius;

  display.setCursor(x, y);
  display.print(logo);

  gAngle += kSpeed;
  if (gAngle >= 2 * PI) {
    gAngle = 0;
  }

  delay(30);
}

}  // namespace DisplayManager
