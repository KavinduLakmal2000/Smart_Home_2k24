void connectionCheck()
{
  if (WiFi.status() != WL_CONNECTED)
  {

    // ---------- Retry Control ----------
    if (wifiRetryCount < 3)
    {
      WiFi.begin(ssid, pass);
      wifiRetryCount++;
    }
    else
    {
      if (millis() - lastWiFiRetry >= retryInterval)
      {
        WiFi.begin(ssid, pass);
        lastWiFiRetry = millis();
      }
    }
    // -----------------------------------

    // Your OLED + LED code (unchanged)
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 0);
    display.print(F("- Smart House -"));
    display.setCursor(45, 9);
    display.print(F("- 2k24 -"));
    display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
    display.setCursor(15, 25);
    display.print(F("WiFi Disconnected"));
    display.setCursor(40, 35);
    display.print(F("Retrying..."));
    display.display();

    blinkRed(); // Call the red blinking function
  }

  else if (!Blynk.connected())
  {

    // ---------- Retry Control ----------
    if (blynkRetryCount < 3)
    {
      Blynk.connect(1000); 
      blynkRetryCount++;
    }
    else
    {
      if (millis() - lastBlynkRetry >= retryInterval)
      {
        Blynk.connect();
        lastBlynkRetry = millis();
      }
    }
    // -----------------------------------

    // Your OLED + LED code (unchanged)
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 0);
    display.print(F("- Smart House -"));
    display.setCursor(45, 9);
    display.print(F("- 2k24 -"));
    display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
    display.setCursor(15, 25);
    display.print(F("Blynk Down!!!"));
    display.setCursor(40, 35);
    display.print(F("Retrying..."));
    display.display();

    blinkYellow(); // Call the yellow blinking function
  }

  else
  {
    if (VoltSensor < 200)
    {
      display3();
      display.display();
    }

    else
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(20, 0);
      display.print(F("- Smart House -"));
      display.setCursor(0, 0);
      display.print(F("K"));
      display.setCursor(43, 9);
      display.printf("%02d:%02d:%02d", Hours, MiN, sec);
      display.setCursor(0, 9);
      display.print(F("L"));
      display.setCursor(122, 0);
      display.print(F("2"));
      display.setCursor(122, 9);
      display.print(F("4"));
      display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);

      switch (currentDisplay)
      {
      case 0:
        display1();
        break;
      case 1:
        display2();
        break;
      case 2:
        display3();
        break;
      case 3:
        display4();
        break;
      }

      display.display();
    }
    wifiRetryCount = 0;
    blynkRetryCount = 0;
    lastWiFiRetry = millis();
    lastBlynkRetry = millis();

    Blynk.run();
    timeDateUpdate();

    Blynk.virtualWrite(V8, batteryVoltage_main); // 16v battery gauge
    Blynk.virtualWrite(V9, VoltSensor);          // 230v ac gauge
    Blynk.virtualWrite(V10, envT);               // tmp gauge

    if (resetCounter > 1000000)
    {
      Blynk.virtualWrite(V2, resetCounter);
      rgbLed.setPixelColor(0, rgbLed.Color(10, 5, 0));
      rgbLed.show();
    }
  }
}


void blinkYellow()
{
  if (millis() - lastBlink > 1000)
  {
    lastBlink = millis();
    ledState2 = !ledState2;

    if (ledState2)
      rgbLed.setPixelColor(0, rgbLed.Color(255, 165, 0));
    else
      rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));

    rgbLed.show();
  }
}

void blinkRed()
{
  if (millis() - lastBlink > 1000)
  {
    lastBlink = millis();
    ledState2 = !ledState2;

    if (ledState2)
      rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));
    else
      rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));

    rgbLed.show();
  }
}