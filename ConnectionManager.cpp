#include "ConnectionManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>

namespace ConnectionManager {

namespace {

void drawStatusScreen(Adafruit_SSD1306& display, int screenWidth, int screenHeight, const __FlashStringHelper* title, const __FlashStringHelper* subtitle) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(22, 0);
  display.print(F("- Smart House -"));
  display.setCursor(45, 9);
  display.print(F("- 2k24 -"));
  display.drawLine(0, screenHeight / 3.6, screenWidth, screenHeight / 3.6, SSD1306_WHITE);
  display.setCursor(15, 25);
  display.print(title);
  display.setCursor(40, 35);
  display.print(subtitle);
  display.display();
}

void blinkStatusLed(Adafruit_NeoPixel& rgbLed, uint32_t color) {
  rgbLed.setPixelColor(0, color);
  rgbLed.show();
  delay(100);
  rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));
  rgbLed.show();
  delay(1000);
}

}  // namespace

void run(ConnectionContext& context) {
  if (WiFi.status() != WL_CONNECTED) {
    if (context.wifiRetryCount < 3) {
      WiFi.begin(context.ssid, context.pass);
      context.wifiRetryCount++;
    } else if (millis() - context.lastWiFiRetry >= context.retryInterval) {
      WiFi.begin(context.ssid, context.pass);
      context.lastWiFiRetry = millis();
    }

    drawStatusScreen(context.display, context.screenWidth, context.screenHeight, F("WiFi Disconnected"), F("Retrying..."));
    blinkStatusLed(context.rgbLed, context.rgbLed.Color(255, 0, 0));
    return;
  }

  if (!Blynk.connected()) {
    if (context.blynkRetryCount < 3) {
      Blynk.connect();
      context.blynkRetryCount++;
    } else if (millis() - context.lastBlynkRetry >= context.retryInterval) {
      Blynk.connect();
      context.lastBlynkRetry = millis();
    }

    drawStatusScreen(context.display, context.screenWidth, context.screenHeight, F("Blynk Down!!!"), F("Retrying..."));
    blinkStatusLed(context.rgbLed, context.rgbLed.Color(255, 165, 0));
    return;
  }

  context.renderConnectedDisplay();
  context.wifiRetryCount = 0;
  context.blynkRetryCount = 0;
  context.lastWiFiRetry = millis();
  context.lastBlynkRetry = millis();

  Blynk.run();
  context.timeDateUpdate();

  Blynk.virtualWrite(V8, context.batteryVoltageMain);
  Blynk.virtualWrite(V9, context.voltSensor);
  Blynk.virtualWrite(V10, context.envT);

  if (context.resetCounter > 600000) {
    Blynk.virtualWrite(V2, context.resetCounter);
    context.rgbLed.setPixelColor(0, context.rgbLed.Color(10, 5, 0));
    context.rgbLed.show();
  }
}

}  // namespace ConnectionManager
