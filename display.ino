void display4() {
  int cx = SCREEN_WIDTH / 2;
  int cy = (SCREEN_HEIGHT / 2) + 20;  // shift down

  const char* logo = "KL";
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  float radius = 10;
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(logo, 0, 0, &x1, &y1, &w, &h);

  int x = cx - (w / 2) + cos(angle) * radius;
  int y = cy - (h / 2) + sin(angle) * radius;

  display.setCursor(x, y);
  display.print(logo);

  angle += speed;
  if (angle >= 2 * PI) angle = 0;

  delay(30);
}

void display3() {
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
  display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);

  display.setCursor(0, 20);
  display.print(F("Main Volt:"));
  display.setCursor(80, 20);
  display.print(VoltSensor);
  display.setCursor(112, 20);
  display.print(F(" v"));

  display.setCursor(0, 30);
  display.print(F("System Volt:"));
  display.setCursor(80, 30);
  display.print(batteryVoltage_sys);
  display.setCursor(112, 30);
  display.print(F(" v"));

  display.setCursor(0, 40);
  display.print(F("Battery Volt:"));
  display.setCursor(80, 40);
  display.print(batteryVoltage_main);
  display.setCursor(112, 40);
  display.print(F(" v"));

  display.setCursor(0, 50);
  display.print(F("Amps:"));
  display.setCursor(80, 50);
  display.print(AmpSensor);
  display.setCursor(112, 50);
  display.print(F(" A"));
}

void display2() {
  display.setCursor(0, 20);
  display.print(F("Main Volt:"));
  display.setCursor(80, 20);
  display.print(VoltSensor);
  display.setCursor(112, 20);
  display.print(F(" v"));

  display.setCursor(0, 30);
  display.print(F("System Volt:"));
  display.setCursor(80, 30);
  display.print(batteryVoltage_sys);
  display.setCursor(112, 30);
  display.print(F(" v"));

  display.setCursor(0, 40);
  display.print(F("Battery Volt:"));
  display.setCursor(80, 40);
  display.print(batteryVoltage_main);
  display.setCursor(112, 40);
  display.print(F(" v"));

  display.setCursor(0, 50);
  display.print(F("Amps:"));
  display.setCursor(80, 50);
  display.print(AmpSensor);
  display.setCursor(112, 50);
  display.print(F(" A"));
}

void display1() {
  display.setCursor(0, 20);
  display.print(F("System Temp: "));
  display.setCursor(80, 20);
  display.print(systemTemp);
  display.print(F(" C"));
  display.setCursor(0, 30);
  display.print(F("Battery Temp: "));
  display.setCursor(80, 30);
  display.print(batteryTemp);
  display.print(F(" C"));
  display.setCursor(0, 40);
  display.print(F("Env Temp: "));
  display.setCursor(80, 40);
  display.print(envT);
  display.print(F(" C"));
  display.setCursor(0, 50);
  display.print(F("Env Humidity: "));
  display.setCursor(80, 50);
  display.print(envH);
  display.print(F(" C"));
}