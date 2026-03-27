DisplayManager::DisplayData getDisplayData() {
  return {
    VoltSensor,
    batteryVoltage_sys,
    batteryVoltage_main,
    AmpSensor,
    systemTemp,
    batteryTemp,
    envT,
    envH
  };
}

AutoSecurityManager::AutoSecurityContext getAutoSecurityContext() {
  return {
    Hours,
    outsidePir_top
  };
}

SecurityModeManager::SecurityModeContext getSecurityModeContext() {
  return {
    SecuMode,
    pirCount1,
    alarmCutOff,
    insidePir,
    outsidePir_top,
    pcf1,
    alarm,
    rf1,
    rf2,
    rf3,
    rf4,
    RF_CH,
    KLroomCh,
    stairsCh,
    livingCh,
    diningCh,
    kitchenCh,
    LedAllOff
  };
}

XModeManager::XModeContext getXModeContext() {
  return {
    xX,
    flag1,
    millisCount,
    ledState,
    insidePir,
    rgbLed
  };
}

SafetySystemManager::SafetySystemContext getSafetySystemContext() {
  return {
    systemTemp,
    batteryTemp,
    envT,
    VoltSensor,
    batteryVoltage_main,
    batteryVoltage_sys,
    rainSensor,
    rainDetectCount,
    systemTempCount,
    batteryTempCount,
    envTCount,
    voltHighCount,
    voltLowCount,
    batteryMainHighCount,
    batteryMainLowCount,
    batterySysHighCount,
    batterySysLowCount,
    fireDetectionCount,
    rainCounter,
    sFlag1,
    sFlag2,
    sFlag3,
    sFlag4,
    sFlag5,
    sFlag6,
    sFlag7,
    sFlag8,
    sFlag9,
    sFlag10,
    rainProtect,
    logRain,
    timestamp,
    pcf1,
    fan,
    lightninProtect,
    pwrCut,
    LedAllOff,
    addError
  };
}

AutoLightManager::AutoLightContext getAutoLightContext() {
  return {
    Hours,
    VoltSensor,
    xX,
    autolightEnable,
    autolightTriggered,
    flag3,
    flag6,
    flag7,
    toggleState,
    pirPreviouslyDetected,
    pirCounter,
    insidePir,
    KLroomCh,
    stairsCh,
    livingCh,
    diningCh,
    kitchenCh,
    rgbLed,
    LedAllOff
  };
}

IRSwitchManager::IRSwitchContext getIRSwitchContext() {
  return {
    irSensor,
    flag5,
    pcf1,
    RF_CH,
    rf3
  };
}

SerialReaderManager::SerialReaderContext getSerialReaderContext() {
  return {
    VoltSensor,
    AmpSensor,
    microControllVolt_analog,
    batteryVoltage_main,
    powerW,
    irSensor,
    thunder,
    rainSensor,
    rainCounter,
    maxSystemBatteryVoltage,
    maxADCValue,
    batteryVoltage_sys
  };
}

void serialRead() {
  SerialReaderManager::SerialReaderContext serialReaderContext = getSerialReaderContext();
  SerialReaderManager::run(serialReaderContext);
}

TemperatureReaderManager::TemperatureReaderContext getTemperatureReaderContext() {
  return {
    dht,
    dht2,
    dht3,
    lastDHTReadTime,
    dhtInterval,
    envH,
    systemTemp,
    batteryTemp,
    envT,
    dhtchk,
    dhtchk1,
    dhtchk2
  };
}
