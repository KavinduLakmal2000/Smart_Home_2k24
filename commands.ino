BLYNK_WRITE(V2)
{ // ----------------------------------------------------------------------------------------- cmd command reading -----------------------
  cmd = param.asStr();

  if (cmd.startsWith("#"))
  {
    Serial.println(cmd);
    delay(500);
    if(cmd == "#idle"){
      Blynk.virtualWrite(V2, "KL idle animation on");
    }
    else if(cmd == "#off"){
      Blynk.virtualWrite(V2, "KL light off");
    }
    else if(cmd == "#vu"){
      Blynk.virtualWrite(V2, "KL VU meter mode");
    }
    else if(cmd == "#tail"){
      Blynk.virtualWrite(V2, "KL tail effect on");
    }
    else if(cmd == "#alarmOn"){
      Blynk.virtualWrite(V2, "Alarm ON");
    }
    else if(cmd == "#alarmOff"){
      Blynk.virtualWrite(V2, "Alarm OFF");
    }
  }

  if (cmd.startsWith("setRain"))
  {
    String valStr = cmd.substring(7); // Extract numbers after "setRain"
    if (valStr.length() > 0)
    {
      int newLimit = valStr.toInt();
      if (SetSetting("rain", newLimit))
      {
        rainThreshold = newLimit;
        Blynk.virtualWrite(V2, "Rain threshold saved: " + String(rainThreshold));
      }
    }
  }

  if (cmd == "pCut")
  {
    pcf1.digitalWrite(pwrCut, LOW);
    delay(1000);
    pcf1.digitalWrite(pwrCut, HIGH);
    Serial_Read();
    delay(1000);
    if (VoltSensor < 30)
    {
      Blynk.virtualWrite(V2, "Main Power is off!");
    }
    else
    {
      Blynk.virtualWrite(V2, "Power Cut-off is faild!");
    }
  }

  if (cmd == "test")
  {
    testV2cmd = true;
  }
  else
  {
    testV2cmd = false;
  }

  if (cmd == "sftyOn")
  {
    safetyMode = true;
  }
  if (cmd == "sftyOff")
  {
    safetyMode = false;
  }

  if (cmd == "getAmps")
  {
    cmd_systemAmp = true;
  }
  else
  {
    cmd_systemAmp = false;
  }

  if (cmd == "getRain")
  {
    Blynk.virtualWrite(V2, "Saved Rain Limit: " + String(rainThreshold));
    delay(1000);
    cmd_rainSensor = true;
  }
  else
  {
    cmd_rainSensor = false;
  }

  if (cmd == "getSystemVolt")
  {
    cmd_systemVolt = true;
  }
  else
  {
    cmd_systemVolt = false;
  }

  if (cmd == "getSystemTemp")
  {
    cmd_sysTemp = true;
  }
  else
  {
    cmd_sysTemp = false;
  }

  if (cmd == "getBatteryTemp")
  {
    cmd_batTemp = true;
  }
  else
  {
    cmd_batTemp = false;
  }

  if (cmd == "testInPir")
  {
    cmd_testPir1 = true;
  }
  else
  {
    cmd_testPir1 = false;
  }

  if (cmd == "rstCount")
  {
    cmd_rstCount = true;
  }
  else
  {
    cmd_rstCount = false;
  }

  if (cmd == "testOutTopPir")
  {
    cmd_testPir2 = true;
  }
  else
  {
    cmd_testPir2 = false;
  }

  if (cmd == "offRain")
  {
    Blynk.virtualWrite(V2, "Automatic lightning protection off!");
    thunder = false;
  }

  if (cmd == "onRain")
  {
    Blynk.virtualWrite(V2, "Automatic lightning protection on!");
    thunder = true;
  }

  if (cmd == "ELightOff")
  {
    pirCounter = 0;
    LedAllOff();
  }

  if (cmd == "Xauto")
  {
    if (SetSetting("Xmode", HIGH))
    {
      Blynk.virtualWrite(V2, "New settings saved!");
    }
    else
    {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "Xmanual")
  {
    if (SetSetting("Xmode", LOW))
    {
      Blynk.virtualWrite(V2, "New settings saved!");
    }
    else
    {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "MidNightAutoLightOn")
  {
    if (SetSetting("m_AutoLight", HIGH))
    {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_m_autoLight = true;
    }
    else
    {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "MidNightAutoLightOff")
  {
    if (SetSetting("m_AutoLight", LOW))
    {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_m_autoLight = false;
    }
    else
    {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "AutoLightOn")
  {
    if (SetSetting("AutoLight", HIGH))
    {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_autoLight = true;
    }
    else
    {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "AutoLightOff")
  {
    if (SetSetting("AutoLight", LOW))
    {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_autoLight = false;
    }
    else
    {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "getSettings")
  {
    SendSettingsToCmd();
  }

  if (cmd == "chkSD")
  {
    checkCardSpace();
  }

  if (cmd == "sftyLog")
  {
    sendLastLinesFromFile("/Safety_system_Error_log.csv", "Error Log");
    sendLastLinesFromFile("/Safety_system_Status_log.csv", "Status Log");
  }

  if (cmd == "firmLog")
  {
    sendFirmwareLog();
  }

  if (cmd == "about")
  {
    sendAboutInfo();
  }

  if (cmd == "pwrGon")
  {
    pcf1.digitalWrite(6, LOW);
    delay(100);
    if (pcf1.digitalRead(6) == LOW)
    {
      Blynk.virtualWrite(V2, "Lightning protection on");
    }
    else
    {
      Blynk.virtualWrite(V2, "Lightning protection failed!");
    }
  }

  if (cmd == "pwrGoff")
  {
    pcf1.digitalWrite(6, HIGH);
    delay(100);
    if (pcf1.digitalRead(6) == HIGH)
    {
      Blynk.virtualWrite(V2, "Lightning protection off");
    }
    else
    {
      Blynk.virtualWrite(V2, "Lightning protection failed!");
    }
  }

  if (cmd == "fan")
  {
    pcf1.digitalWrite(fan, HIGH);
    delay(5000);
    pcf1.digitalWrite(fan, LOW);
    Blynk.virtualWrite(V2, "Fan Checked!");
  }

  if (cmd == "p")
  {
    pirCounter = 0;
    pirPreviouslyDetected = false;
    toggleState = false;
    Blynk.virtualWrite(V2, "PIR count reset!");
  }

  if (cmd == "reset")
  {
    Blynk.virtualWrite(V2, "Restarting ESP32...");
    delay(1000);
    esp_restart();
  }
  if (cmd == "rf1On")
  {
    pcf1.digitalWrite(rf1, LOW);
    Blynk.virtualWrite(V2, "RF SW1 ON");
  }
  if (cmd == "rf1Off")
  {
    pcf1.digitalWrite(rf1, HIGH);
    Blynk.virtualWrite(V2, "RF SW1 OFF");
  }

  if (cmd == "rf2On")
  {
    pcf1.digitalWrite(rf2, LOW);
    Blynk.virtualWrite(V2, "RF SW2 ON");
  }
  if (cmd == "rf2Off")
  {
    pcf1.digitalWrite(rf2, HIGH);
    Blynk.virtualWrite(V2, "RF SW2 OFF");
  }

  if (cmd == "rf3On")
  {
    pcf1.digitalWrite(rf3, LOW);
    Blynk.virtualWrite(V2, "RF SW3 ON");
  }
  if (cmd == "rf3Off")
  {
    pcf1.digitalWrite(rf3, HIGH);
    Blynk.virtualWrite(V2, "RF SW3 OFF");
  }

  if (cmd == "rf4On")
  {
    pcf1.digitalWrite(rf4, LOW);
    Blynk.virtualWrite(V2, "RF SW4 ON");
  }
  if (cmd == "rf4Off")
  {
    pcf1.digitalWrite(rf4, HIGH);
    Blynk.virtualWrite(V2, "RF SW4 OFF");
  }

  if (cmd == "rfCH1")
  {
    pcf1.digitalWrite(RF_CH, HIGH);
    Blynk.virtualWrite(V2, "RF Channel 1");
  }

  if (cmd == "rfCH2")
  {
    pcf1.digitalWrite(RF_CH, LOW);
    Blynk.virtualWrite(V2, "RF Channel 2");
  }

  if (cmd == "rfAllOff")
  {
    pcf1.digitalWrite(rf1, HIGH);
    pcf1.digitalWrite(rf2, HIGH);
    pcf1.digitalWrite(rf3, HIGH);
    pcf1.digitalWrite(rf4, HIGH);
    delay(500);
    pcf1.digitalWrite(RF_CH, HIGH);
    delay(500);
    pcf1.digitalWrite(RF_CH, LOW);
    Blynk.virtualWrite(V2, "All RF lights OFF");
  }

  if (cmd == "rfAllOn")
  {
    pcf1.digitalWrite(rf1, LOW);
    pcf1.digitalWrite(rf2, LOW);
    pcf1.digitalWrite(rf3, LOW);
    pcf1.digitalWrite(rf4, LOW);
    delay(500);
    pcf1.digitalWrite(RF_CH, HIGH);
    delay(500);
    pcf1.digitalWrite(RF_CH, LOW);
    Blynk.virtualWrite(V2, "All RF lights ON");
  }

  if (cmd == "rfStatus")
  {

    Blynk.virtualWrite(V2, "RF Status Report");
    delay(100);

    if (pcf1.digitalRead(RF_CH))
    { //////////////////////////// rf ch 1
      Blynk.virtualWrite(V2, "RF Channel 1 (Ground Floor)");
      delay(100);

      if (pcf1.digitalRead(rf1))
      {
        Blynk.virtualWrite(V2, "RF SW1 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW1 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf2))
      {
        Blynk.virtualWrite(V2, "RF SW2 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW2 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf3))
      {
        Blynk.virtualWrite(V2, "RF SW3 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW3 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf4))
      {
        Blynk.virtualWrite(V2, "RF SW4 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW4 ON");
      }
    }

    else
    { //////////////////////////////////////////////// rf ch 2
      Blynk.virtualWrite(V2, "RF Channel 2 (Mid Floor)");
      delay(100);

      if (pcf1.digitalRead(rf1))
      {
        Blynk.virtualWrite(V2, "RF SW1 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW1 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf2))
      {
        Blynk.virtualWrite(V2, "RF SW2 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW2 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf3))
      {
        Blynk.virtualWrite(V2, "RF SW3 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW3 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf4))
      {
        Blynk.virtualWrite(V2, "RF SW4 OFF");
      }
      else
      {
        Blynk.virtualWrite(V2, "RF SW4 ON");
      }
    }
  }

  if (cmd == "topLightOn")
  {
    topLight = true;
    Blynk.virtualWrite(V2, "Top floor light ON");
  }

  if (cmd == "topLightOff")
  {
    topLight = false;
    Blynk.virtualWrite(V2, "Top floor light OFF");
  }

  if (cmd == "setTopLightOn")
  {
    if (SetSetting("topLightAuto", HIGH))
    {
      Local_topLightAuto = true;
      Blynk.virtualWrite(V2, "Top light schedule saved: ON");
    }
    else { Blynk.virtualWrite(V2, "Failed to save setting"); }
  }

  if (cmd == "setTopLightOff")
  {
    if (SetSetting("topLightAuto", LOW))
    {
      Local_topLightAuto = false;
      Blynk.virtualWrite(V2, "Top light schedule saved: OFF");
    }
    else { Blynk.virtualWrite(V2, "Failed to save setting"); }
  }

  if (cmd == "Xon")
  {
    if (GetSettings("Xmode"))
    {
      Blynk.virtualWrite(V2, "Auto X mode is on!");
    }
    xX = true;
    flag1 = true;
    sFlag11 = true;
    SetSetting("Xmode", HIGH);
  }
  if (cmd == "Xoff")
  {
    xX = false;
    flag1 = true;
    SetSetting("Xmode", LOW);
  }

  if (cmd == "time")
  {
    cmd_time = true;
  }
  else
  {
    cmd_time = false;
  }

  if (cmd == "help")
  {
    Blynk.virtualWrite(V2, "📖 Command Help Menu:");
    delay(100);

    Blynk.virtualWrite(V2, "pCut - Cut main power temporarily");
    Blynk.virtualWrite(V2, "test - Enable test mode");
    Blynk.virtualWrite(V2, "sftyOn / sftyOff - Safety mode ON/OFF");
    Blynk.virtualWrite(V2, "-------------------------------------------");
    Blynk.virtualWrite(V2, "getAmps - Get system current");
    Blynk.virtualWrite(V2, "getRain - Get rain sensor status");
    Blynk.virtualWrite(V2, "setRain1023 - Set rain sensor sensitivity");
    Blynk.virtualWrite(V2, "getSystemVolt - Get system voltage");
    Blynk.virtualWrite(V2, "getSystemTemp - Get system temperature");
    Blynk.virtualWrite(V2, "getBatteryTemp - Get battery temperature");
    Blynk.virtualWrite(V2, "getSettings - Show current settings");
    Blynk.virtualWrite(V2, "-------------------------------------------");
    Blynk.virtualWrite(V2, "testInPir / testOutTopPir - PIR test modes");
    Blynk.virtualWrite(V2, "rstCount - Reset counter");
    Blynk.virtualWrite(V2, "ELightOff - Emergency lights OFF");
    Blynk.virtualWrite(V2, "chkSD - Check SD card space");
    Blynk.virtualWrite(V2, "time - Show system current time");
    Blynk.virtualWrite(V2, "-------------------------------------------");
    Blynk.virtualWrite(V2, "sftyLog - Send safety logs");
    Blynk.virtualWrite(V2, "firmLog - Send firmware log");
    Blynk.virtualWrite(V2, "-------------------------------------------");
    Blynk.virtualWrite(V2, "p - pir count reset");
    Blynk.virtualWrite(V2, "about - Show about info");
    Blynk.virtualWrite(V2, "pwrGon / pwrGoff - Lightning protection ON/OFF");
    Blynk.virtualWrite(V2, "fan - Test fan");
    Blynk.virtualWrite(V2, "reset - Restart ESP32");
    Blynk.virtualWrite(V2, "topLightOn / topLightOff - Top floor light control");
    Blynk.virtualWrite(V2, "-------------------------------------------");
    Blynk.virtualWrite(V2, "rf1On / rf1Off ... rf4On / rf4Off - RF Switch control");
    Blynk.virtualWrite(V2, "rfCH1 / rfCH2 - Select RF Channel");
    Blynk.virtualWrite(V2, "rfAllOn / rfAllOff - RF lights ON/OFF");
    Blynk.virtualWrite(V2, "rfStatus - Get RF status report");
    Blynk.virtualWrite(V2, "-------------------------------------------");
    Blynk.virtualWrite(V2, "AutoLightOn / AutoLightOff - Night time emergency light ON/OFF");
    Blynk.virtualWrite(V2, "MidNightAutoLightOn / MidNightAutoLightOff - Mid night auto light ON/OFF");
    Blynk.virtualWrite(V2, "setTopLightOn / setTopLightOff - Top light schedule ON/OFF");
    Blynk.virtualWrite(V2, "-------------------------------------------");
    Blynk.virtualWrite(V2, "# - Send custom command to serial");
    Blynk.virtualWrite(V2, "#idle - idle animation on KL");
    Blynk.virtualWrite(V2, "#off - KL light off");
    Blynk.virtualWrite(V2, "#vu - KL Works as VU meter");
    Blynk.virtualWrite(V2, "#tail - KL light tail effect");
    Blynk.virtualWrite(V2, "#alarmOn - alarm on");    
    Blynk.virtualWrite(V2, "#alarmOff - alarm off");    
  }
}