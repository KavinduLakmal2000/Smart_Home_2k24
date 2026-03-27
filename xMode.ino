void x_Mode() {
  if (xX) {

    if (flag1) {
      Blynk.virtualWrite(V2, "XXX mode is on");
      flag1 = false;
    }

    if (digitalRead(insidePir)) {
      if (millis() - millisCount >= 50) {
        millisCount = millis();

        ledState = !ledState;

        if (ledState) {
          rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 255));  // Red ON
          rgbLed.show();
          delay(500);
        } else {
          rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // OFF
          rgbLed.show();
        }
      }

    } else {
      rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // Red
      rgbLed.show();
    }

  }

  else {

    if (flag1) {
      Blynk.virtualWrite(V2, "XXX mode is off");
      flag1 = false;
    }

    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // Red
    rgbLed.show();
  }
}
