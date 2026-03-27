//////////////////////////////////////////

// i2c Address :
// INA226 sensor - 0x44
// PCF8575 - 0x23
// OLED Display - ox3c

///////////////////////////////////////
const char* SoftVer = "Firmware 3.1.1";
const char* whatsNew = "Add feature to turn off and on for lightning protection.)";
////////////////////TEST MODE///////////////////
bool testMode = false;
bool safetyMode = true;
////////////////////////////////////////////////


#define BLYNK_TEMPLATE_ID "TMPL6JKpqPS9E"
#define BLYNK_TEMPLATE_NAME "ESP32 Smart Home"
#define BLYNK_AUTH_TOKEN "Hl5yD-nmduiUih1XUFzHqgPOmrr1nZRT"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include "Arduino.h"
#include "PCF8575.h"
#include "esp_system.h"
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTTYPE DHT11
#define dht_dpin 42  // D4

#define DHTTYPE2 DHT11  // DHT 11
#define dht_dpin2 14    // D4

#define DHTTYPE3 DHT11
#define dht_dpin3 41


DHT dht(dht_dpin, DHTTYPE);
DHT dht2(dht_dpin2, DHTTYPE2);
DHT dht3(dht_dpin3, DHTTYPE3);

#define SD_MISO 37
#define SD_MOSI 35
#define SD_SCK 36
#define SD_CS 1

SPIClass spi = SPIClass(FSPI);

PCF8575 pcf1(0x23);  // 0x21

#define LED_PIN 48
#define NUM_PIXELS 1
Adafruit_NeoPixel rgbLed(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SLT_Fiber_Optic";
char pass[] = "Life1Mal7i";

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800);
int lastMinute = -1;
int lastCheckedDay = -1;
char timestamp[25];

int Hours = 0;
int MiN = 0;
int sec = 0;

float angle = 0.0;
float speed = 0.05;  // smaller = logo slower

int i = 0;

#define KLroomPin 2
#define stairsPin 3
#define livingPin 6
#define diningPin 4
#define kitchenPin 10

#define KLroomCh 0
#define stairsCh 1
#define livingCh 2
#define diningCh 3
#define kitchenCh 4

// Common PWM settings
const int freq = 5000;     // 5 kHz
const int resolution = 8;  // 8-bit resolution (0-255)

String cmd = "";
bool xX = false;
unsigned long millisCount = 0;
bool ledState = false;
bool SecuMode = false;
int pirCount1 = 0;
int pirCounter = 0;
int alarmCutOff = true;
bool cmd_systemVolt = false;
bool cmd_sysTemp = false;
bool cmd_batTemp = false;
bool cmd_systemAmp = false;
bool cmd_rainSensor = false;
bool cmd_rstCount = false;

unsigned long resetCounter = 0;

bool flag1 = false;
bool flag3 = false;
bool flag4 = false;
bool flag5 = false;
bool flag6 = false;
bool flag7 = false;
bool flag8 = false;
bool toggleState = false;
bool pirPreviouslyDetected = false;

unsigned long previousMillis = 0;
const long display_interval = 5000;  // 5 seconds
int currentDisplay = 0;              // Track which display is active

unsigned long lastDHTReadTime = 0;       // Store last read time
const unsigned long dhtInterval = 2000;  // 2-second interval

String irSensor = "o";
float VoltSensor = 0;
float AmpSensor = 0;
float powerW = 0;
float microControllVolt_analog = 0;
float mainBatteryVolt_analog = 0;
bool audio = false;
bool autolightEnable = false;
bool autolightTriggered = false;
bool main230Out = false;
bool irEn = false;
bool rainProtect = false;

const float maxSystemBatteryVoltage = 6.0;
const int maxADCValue = 1023;
float batteryVoltage_sys = 0;
float batteryVoltage_main = 0;
int rainSensor = 1023;
int rainDetectCount = 0;
bool thunder = true;

bool sFlag1 = false;
bool sFlag2 = false;
bool sFlag3 = false;
bool sFlag4 = false;
bool sFlag5 = false;
bool sFlag6 = false;
bool sFlag7 = false;
bool sFlag8 = false;
bool sFlag9 = false;
bool sFlag10 = false;
bool sFlag11 = false;
bool sFlag12 = false;


long systemTempCount = 0;
long batteryTempCount = 0;
long envTCount = 0;
long voltHighCount = 0;
long voltLowCount = 0;
long batteryMainHighCount = 0;
long batteryMainLowCount = 0;
long batterySysHighCount = 0;
long batterySysLowCount = 0;
long fireDetectionCount = 0;
long rainCounter = 0;

float systemTemp = 0;
float batteryTemp = 0;
float envH = 0;
float envT = 0;
long dhtchk = 0;
long dhtchk1 = 0;
long dhtchk2 = 0;

int wifiRetryCount = 0;
int blynkRetryCount = 0;
unsigned long lastWiFiRetry = 0;
unsigned long lastBlynkRetry = 0;
const unsigned long retryInterval = 10UL * 60UL * 1000UL;  // 10 minutes

bool Local_m_autoLight = false;
bool Local_autoLight = false;

bool cmd_testPir1 = false;
bool cmd_testPir2 = false;

#define insidePir 13       // esp pin
#define outsidePir_top 12  // esp pin
#define outsidePir_bot 11  //esp pin
#define alarm 2            // pcf
#define fan 3              // pcf pin
#define lightninProtect 6  //pcf
#define pwrCut 7           // pcf
#define espEnable 5        // esp
#define ir_pin 4           // pcf

#define rf1 8
#define rf2 9
#define rf3 10
#define rf4 11

#define RF_CH 12

String logRain = "";
String logMainVoltStatus = "";
String errorBuffer = "";
#define MAX_LINES 30

bool testV2cmd = false;

void LedAllOff() {
  ledcWrite(KLroomCh, 0);
  ledcWrite(stairsCh, 0);
  ledcWrite(livingCh, 0);
  ledcWrite(diningCh, 0);
  ledcWrite(kitchenCh, 0);

  Blynk.virtualWrite(V1, 0);  // KL ROOM
  Blynk.virtualWrite(V6, 0);  // Dining
  Blynk.virtualWrite(V7, 0);  // Kitchin
  Blynk.virtualWrite(V4, 0);  // Stairs
  Blynk.virtualWrite(V5, 0);  // Living
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  unsigned long wifiStart = millis();
  bool wifiConnected = false;

  bool displayInitialized = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if (!displayInitialized) {
    Serial.println(F("⚠️ SSD1306 allocation failed. Continuing without display..."));
  }

  rgbLed.begin();
  rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));  // Red ON
  rgbLed.show();

  // --- OLED WiFi Connecting Animation ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(22, 0);
  display.print(F("- Smart House -"));
  display.setCursor(45, 9);
  display.print(F("- 2k24 -"));
  display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
  display.setCursor(20, 25);
  display.print(F("Connecting WiFi"));
  display.display();


  int dotX = 115;
  while (millis() - wifiStart < 10000) {
    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
      break;
    }

    if (displayInitialized) {
      display.fillRect(dotX, 25, 2, 2, SSD1306_WHITE);  // animation dots
      display.display();
      dotX += 2;
      if (dotX > 125) dotX = 115;
    }

    delay(250);
  }

  // --- WiFi Connection Result ---
  if (wifiConnected) {
    Serial.println("✅ WiFi Connected!");
    if (displayInitialized) {
      display.setCursor(25, 35);
      display.print(F("WiFi Connected!"));
      display.display();
    }
    Blynk.config(auth);
    Blynk.connect();
  } else {
    Serial.println("❌ WiFi Failed. Retrying in loop.");
    if (displayInitialized) {
      display.setCursor(10, 35);
      display.print(F("Failed. Retrying..."));
      display.display();
    }
    WiFi.begin(ssid, pass);  // retry automatically
    Blynk.config(auth);      // non-blocking
  }

  // --- Continue other hardware init ---
  pcf1.begin();
  timeClient.begin();
  Wire.begin();
  dht.begin();
  delay(100);
  dht2.begin();
  delay(100);
  dht3.begin();
  Blynk.run();

  rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 200));  // Blue ON
  rgbLed.show();

  esp_reset_reason_t reason = esp_reset_reason();
  Blynk.virtualWrite(V2, reason);
  Blynk.virtualWrite(V2, "System Starting...");

  pinMode(espEnable, INPUT);
  while (!digitalRead(espEnable)) {
    Blynk.virtualWrite(V2, "Connect ESP32 to main board");
    rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));  // Red ON
    rgbLed.show();
    delay(200);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // Red OFF
    rgbLed.show();
    delay(2000);
  }

  // ------------------------------------------------------------------- SD card --------------------------------------------
  spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spi)) {
    Blynk.virtualWrite(V2, "Card Mount Failed");
  } else {
    Blynk.virtualWrite(V2, "SD card initialized successfully!");
  }

  updateAboutFile();
  delay(500);
  logFirmwareVersion();
  delay(100);
  checkCardSpace();
  delay(100);
  sendAboutInfo();
  delay(200);
  if (GetSettings("Xmode") == HIGH) {
    Blynk.virtualWrite(V2, "X on");
  } else {
    Blynk.virtualWrite(V2, "X off");
  }
  delay(100);

  if (GetSettings("AutoLight") == HIGH) {
    Blynk.virtualWrite(V2, "Auto Lights on");
  } else {
    Blynk.virtualWrite(V2, "Auto Lights off");
  }
  delay(100);

  if (GetSettings("m_AutoLight") == HIGH) {
    Blynk.virtualWrite(V2, "Mid Night Auto Lights on");
  } else {
    Blynk.virtualWrite(V2, "Mid Night Auto Lights off");
  }

  delay(100);
  Blynk.virtualWrite(V2, "Settings loaded!");

  //------------------------------------------------------------------------------------------------------------------------

  if (!testMode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 0);
    display.print(F("- Smart House -"));
    display.setCursor(45, 9);
    display.print(F("- 2k24 -"));
    display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
    display.setCursor(20, 20);
    display.println("System Loading...");

    for (int i = 0; i < 122; i++) {
      display.setCursor(i, 40);
      display.println("I");
      display.display();
      delay(5);
    }

    display.display();
  }

  // --- PinMode Setup ---
  ledcSetup(KLroomCh, freq, resolution);
  ledcAttachPin(KLroomPin, KLroomCh);

  ledcSetup(stairsCh, freq, resolution);
  ledcAttachPin(stairsPin, stairsCh);

  ledcSetup(livingCh, freq, resolution);
  ledcAttachPin(livingPin, livingCh);

  ledcSetup(diningCh, freq, resolution);
  ledcAttachPin(diningPin, diningCh);

  ledcSetup(kitchenCh, freq, resolution);
  ledcAttachPin(kitchenPin, kitchenCh);

  pinMode(insidePir, INPUT);
  pinMode(outsidePir_top, INPUT);

  pcf1.pinMode(alarm, OUTPUT);
  pcf1.pinMode(rf1, OUTPUT);
  pcf1.pinMode(rf2, OUTPUT);
  pcf1.pinMode(rf3, OUTPUT);
  pcf1.pinMode(rf4, OUTPUT);
  pcf1.pinMode(RF_CH, OUTPUT);
  pcf1.pinMode(3, OUTPUT);
  pcf1.pinMode(lightninProtect, OUTPUT);
  pcf1.pinMode(pwrCut, OUTPUT);
  pcf1.pinMode(fan, OUTPUT);
  pcf1.pinMode(ir_pin, INPUT);

  pcf1.digitalWrite(lightninProtect, HIGH);
  pcf1.digitalWrite(pwrCut, HIGH);
  pcf1.digitalWrite(fan, LOW);
  pcf1.digitalWrite(alarm, LOW);

  LedAllOff();

  pcf1.digitalWrite(rf1, HIGH);
  pcf1.digitalWrite(rf2, HIGH);
  pcf1.digitalWrite(rf3, HIGH);
  pcf1.digitalWrite(rf4, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, LOW);
  LedAllOff();

  // --- Final display screen ---
  if (!testMode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 0);
    display.print(F("- Smart House -"));
    display.setCursor(45, 9);
    display.print(F("- 2k24 -"));
    display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
    display.setCursor(50, 25);
    display.print(F("Welcome!"));
    display.setCursor(27, 35);
    display.print(SoftVer);

    display.display();
    Serial_Read();
    delay(5000);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));
    rgbLed.show();
    display.clearDisplay();
    display.display();
  }
  pcf1.digitalWrite(rf1, HIGH);
  pcf1.digitalWrite(rf2, HIGH);
  pcf1.digitalWrite(rf3, HIGH);
  pcf1.digitalWrite(rf4, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, LOW);
  LedAllOff();

  delay(100);
}


void loop() {  //------------------------------------------------------------------------------------------------ loop start ------------------------------------------------------------
  resetCounter++;


  Serial_Read();   // read promini serial data
  x_Mode();        // the room x mode
  Securty_mode();  // activate with button, all pir's working at same time, alarm lock off
  readTemp();      // read all the temp sensors and filter output values
                   //autoSecurty();                   // pir sensors at out side automatic work and notifications
  irSwitch();

  if (MiN != lastMinute) {  // write data in SD card every 1 Min
    lastMinute = MiN;
    DataLog();
    flushErrors();
  }
  //--------------------------------------------------------------

  if (Hours == 17 && !sFlag12) {
    if (GetSettings("AutoLight") == HIGH) {
      Local_autoLight = true;
    } else {
      Local_autoLight = false;
    }

    if (GetSettings("m_AutoLight") == HIGH) {
      Local_m_autoLight = true;
    } else {
      Local_m_autoLight = false;
    }
    sFlag12 = true;
  }

  if (Hours == 18 && sFlag12) {
    sFlag12 = false;
  }

  //------------------------------------------------------------

  if (Hours == 1 && !sFlag11) {
    if (GetSettings("Xmode") == HIGH) {
      xX = true;
    }
    sFlag11 = true;
  }

  if (Hours == 7 && sFlag11) {
    xX = false;
    sFlag11 = false;
  }

  //-------------------------------------------------------------

  if (Local_autoLight) {
    autoLight();  // automatic turn on lights when house main power off at 6PM to 10PM
  }

  if (Local_m_autoLight) {
    midNightAutoLights();  // automatic turn on lights when house main power is off at 10PM to 5AM (only when inside pir detects someone)
  }

  if (safetyMode) {
    saftySys();  // checking all the temp sensors and voltage sensors if any problems send notification and cmd lines
    if (flag8) {
      Blynk.virtualWrite(V2, "Safety System Online");
      flag8 = false;
    }

  } else {
    if (!flag8) {
      Blynk.virtualWrite(V2, "Safety System Shut Down!");
      flag8 = true;
    }
  }


  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  if (VoltSensor < 50) {
    //Blynk.logEvent("volts_amps_sensors", "Main 230v Power Supply is off"); //------------ enable this after all done
    main230Out = true;

  }

  else {
    if (main230Out) {
      pcf1.digitalWrite(rf1, HIGH);
      pcf1.digitalWrite(rf2, HIGH);
      pcf1.digitalWrite(rf3, HIGH);
      pcf1.digitalWrite(rf4, HIGH);
      delay(300);
      pcf1.digitalWrite(RF_CH, HIGH);
      delay(300);
      pcf1.digitalWrite(RF_CH, LOW);
      main230Out = false;
    }
  }


  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= display_interval) {
    previousMillis = currentMillis;
    currentDisplay = (currentDisplay + 1) % 2;  // show display count max is 4
  }

  if (testV2cmd) {  // for testing values cmd printing command for active "test"
    //Blynk.virtualWrite(V2, "rain count:");
    //Blynk.virtualWrite(V2, rainCounter);
    Blynk.virtualWrite(V2, "test");
    midNightAutoLights();
  }

  if (cmd_rstCount) {
    Blynk.virtualWrite(V2, resetCounter);
  }

  if (cmd_rainSensor) {
    Blynk.virtualWrite(V2, rainSensor);
  }

  if (cmd_systemAmp) {
    Blynk.virtualWrite(V2, AmpSensor);
  }

  if (cmd_systemVolt) {
    Blynk.virtualWrite(V2, batteryVoltage_sys);
  }

  if (cmd_sysTemp) {
    Blynk.virtualWrite(V2, systemTemp);
  }

  if (cmd_batTemp) {
    Blynk.virtualWrite(V2, batteryTemp);
  }

  if (cmd_testPir1) {
    Blynk.virtualWrite(V2, digitalRead(insidePir));
  }

  if (cmd_testPir2) {
    Blynk.virtualWrite(V2, digitalRead(outsidePir_top));
  }


  if (alarmCutOff) {
    pcf1.digitalWrite(alarm, LOW);
  }

  if (WiFi.status() != WL_CONNECTED) {

    // ---------- Retry Control ----------
    if (wifiRetryCount < 3) {
      WiFi.begin(ssid, pass);
      wifiRetryCount++;
    } else {
      if (millis() - lastWiFiRetry >= retryInterval) {
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

    rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));
    rgbLed.show();
    delay(100);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));
    rgbLed.show();
    delay(1000);
  }


  else if (!Blynk.connected()) {

    // ---------- Retry Control ----------
    if (blynkRetryCount < 3) {
      Blynk.connect();
      blynkRetryCount++;
    } else {
      if (millis() - lastBlynkRetry >= retryInterval) {
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

    rgbLed.setPixelColor(0, rgbLed.Color(255, 165, 0));
    rgbLed.show();
    delay(100);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));
    rgbLed.show();
    delay(1000);
  }


  else {
    if (VoltSensor < 200) {
      display3();
      display.display();
    }


    else {
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

      switch (currentDisplay) {
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

    Blynk.virtualWrite(V8, batteryVoltage_main);  // 16v battery gauge
    Blynk.virtualWrite(V9, VoltSensor);           // 230v ac gauge
    Blynk.virtualWrite(V10, envT);                //tmp gauge

    if (resetCounter > 600000) {
      Blynk.virtualWrite(V2, resetCounter);
      rgbLed.setPixelColor(0, rgbLed.Color(10, 5, 0));
      rgbLed.show();
    }
  }

  delay(300);
}  // ================================================================================================= loop end ==================================================================

//----------------------------------------------------------------------------- SD Card START----------------------------------------------------------------------------------------
void DataLog() {
  /////////////////////////////////////////////////////////////// log ac 230v ////////////////////////////////////////////
  String logMainVolt = String(timestamp) + ", " + String(VoltSensor);
  File ACVolt = SD.open("/AC230v_log.csv", FILE_APPEND);
  if (ACVolt) {
    ACVolt.println(logMainVolt);
    ACVolt.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Ac Volt");
  }

  //////////////////////////////////////////////////////////////// log AC 230 on or off ///////////////////////////////

  if (VoltSensor > 210) {
    logMainVoltStatus = String(timestamp) + ", AC ON";
  } else {
    logMainVoltStatus = String(timestamp) + ", AC OFF";
  }

  File ACVoltSts = SD.open("/AC230v_Status_log.csv", FILE_APPEND);
  if (ACVoltSts) {
    ACVoltSts.println(logMainVoltStatus);
    ACVoltSts.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Ac Volt Status");
  }

  //////////////////////////////////////////////log amp sensor/////////////////////////////////////////////////////

  String logAmpSen = String(timestamp) + ", " + String(AmpSensor);
  File amps = SD.open("/Amp_Sensor_log.csv", FILE_APPEND);
  if (amps) {
    amps.println(logAmpSen);
    amps.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Amp sensor");
  }


  //////////////////////////////////////////////// main battery /////////////////////////////////////////////////////
  String logMainBattery = String(timestamp) + ", " + String(batteryVoltage_main);
  File MainBat = SD.open("/Main_battery_log.csv", FILE_APPEND);
  if (MainBat) {
    MainBat.println(logMainBattery);
    MainBat.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Main Battery");
  }


  //////////////////////////////////////////////// System Battery //////////////////////////////////////////////////
  String logSysBattery = String(timestamp) + ", " + String(batteryVoltage_sys);
  File SysBat = SD.open("/System_battery_log.csv", FILE_APPEND);
  if (SysBat) {
    SysBat.println(logSysBattery);
    SysBat.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log System Battery");
  }


  ////////////////////////////////////////////////// system temp sensor value///////////////////////////////////////////
  String logSysTemp = String(timestamp) + ", " + String(systemTemp);
  File SysTemplog = SD.open("/System_temp_log.csv", FILE_APPEND);
  if (SysTemplog) {
    SysTemplog.println(logSysTemp);
    SysTemplog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log System temp");
  }

  ////////////////////////////////////////////////////// main battery temp value ///////////////////////////////////////////
  String logMainBatTemp = String(timestamp) + ", " + String(batteryTemp);
  File MainBatTemplog = SD.open("/Battery_temp_log.csv", FILE_APPEND);
  if (MainBatTemplog) {
    MainBatTemplog.println(logMainBatTemp);
    MainBatTemplog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Main Battery temp");
  }

  //////////////////////////////////////////////////////// environment temp value //////////////////////////////////////////
  String logEnvTemp = String(timestamp) + ", " + String(envT);
  File EnvTemplog = SD.open("/Environment_temp_log.csv", FILE_APPEND);
  if (EnvTemplog) {
    EnvTemplog.println(logEnvTemp);
    EnvTemplog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Environment temp");
  }

  /////////////////////////////////////////////////////////// Rain Log ////////////////////////////////////////////////////
  File RainLog = SD.open("/Rain_log.csv", FILE_APPEND);
  if (RainLog) {
    RainLog.println(logRain);
    RainLog.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to log Environment Rain");
  }
}

//////////////////////////////////////////////////////////// safety system log ////////////////////////////////

void addError(String message) {
  errorBuffer += String(timestamp) + ", " + message + "\n";
}

void flushErrors() {
  if (errorBuffer.length() > 0) {
    // Write all collected errors
    File SafetyLog = SD.open("/Safety_system_Error_log.csv", FILE_APPEND);
    if (SafetyLog) {
      SafetyLog.print(errorBuffer);
      SafetyLog.close();
    } else {
      Blynk.virtualWrite(V2, "Failed to log Safety system error");
    }
    errorBuffer = "";
  } else {
    String logEntry = String(timestamp) + ", All good";
    File SafetyLog = SD.open("/Safety_system_Status_log.csv", FILE_WRITE);
    if (SafetyLog) {
      SafetyLog.println(logEntry);
      SafetyLog.close();
    } else {
      Blynk.virtualWrite(V2, "Failed to log Safety system status");
    }
  }
}
//------------------------------------------------------ check safety log in blynk cmd ----------------------------------

void sendLastLinesFromFile(const char* filename, String label) {
  File logFile = SD.open(filename, FILE_READ);
  if (!logFile) {
    Blynk.virtualWrite(V2, "Failed to open " + String(filename));
    return;
  }

  String lastLines[MAX_LINES];
  int count = 0;
  String currentLine = "";

  while (logFile.available()) {
    char c = logFile.read();
    if (c == '\n') {
      lastLines[count % MAX_LINES] = currentLine;
      count++;
      currentLine = "";
    } else {
      currentLine += c;
    }
  }

  if (currentLine.length() > 0) {
    lastLines[count % MAX_LINES] = currentLine;
    count++;
  }

  logFile.close();

  int start = (count > MAX_LINES) ? (count - MAX_LINES) : 0;

  Blynk.virtualWrite(V2, "---- " + label + " ----");
  for (int i = start; i < count; i++) {
    Blynk.virtualWrite(V2, lastLines[i % MAX_LINES]);
    delay(50);
  }
}
// --------------------------------------------------- Check SD Card Space ------------------------------------------
unsigned long getDirSize(File dir) {
  unsigned long total = 0;
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    if (entry.isDirectory()) {
      total += getDirSize(entry);
    } else {
      total += entry.size();
    }
    entry.close();
  }
  return total;
}

void checkCardSpace() {
  File root = SD.open("/");
  unsigned long totalSize = getDirSize(root);
  root.close();
  unsigned long totalSizeMB = totalSize / (1024 * 1024);

  // 1.5 GB = 1.5 * 1024 * 1024 * 1024
  if (totalSizeMB > 1536) {
    Blynk.virtualWrite(V2, "⚠️ WARNING: SD card almost full!");
    Blynk.logEvent("volts_amps_sensors", "WARNING: SD card almost full!");
  } else {
    Blynk.virtualWrite(V2, "SD card space OK");
    Blynk.virtualWrite(V2, String(totalSizeMB) + " MB");
  }
}
//---------------------------------------------------------------------- add about.text -------------------------------------------------
// Function to create/update about.txt
void updateAboutFile() {
  File file = SD.open("/about.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open about.txt for writing");
    return;
  }
  file.println(SoftVer);
  file.println("Created BY KLTECHNOLOGY");
  file.println("© 2024 Kavindu Lakmal. All rights reserved");
  file.close();
}

// Function to read and send file via Blynk
void sendAboutInfo() {
  File file = SD.open("/about.txt", FILE_READ);
  if (!file) {
    Blynk.virtualWrite(V2, "⚠️ No about.txt found");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      Blynk.virtualWrite(V2, line);
      delay(150);
    }
  }

  file.close();
}
//-------------------------------------------------------------------------- log firmware data --------------------------------------------------
void logFirmwareVersion() {
  // First check if file already contains this version
  timeDateUpdate();
  delay(1000);
  File file = SD.open("/What_new_log.csv", FILE_READ);
  if (file) {
    String content = "";
    while (file.available()) {
      content = file.readStringUntil('\n');
      if (content.indexOf(SoftVer) != -1) {
        Blynk.virtualWrite(V2, "Firmware is up to date");
        file.close();
        return;
      }
    }
    file.close();
  }

  // If not found → append new version entry
  File logFile = SD.open("/What_new_log.csv", FILE_APPEND);
  if (logFile) {
    String entry = String(timestamp) + ", " + SoftVer + ", " + whatsNew;
    logFile.println(entry);
    Blynk.virtualWrite(V2, "Firmware Updated To New Version");
    logFile.close();
  } else {
    Blynk.virtualWrite(V2, "Failed to write changelog");
  }
}

void sendFirmwareLog() {
  File logFile = SD.open("/What_new_log.csv", FILE_READ);
  if (!logFile) {
    Blynk.virtualWrite(V2, "⚠️ No firmware log found");
    return;
  }

  while (logFile.available()) {
    String line = logFile.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      Blynk.virtualWrite(V2, line);
      delay(120);  // prevent flooding Blynk
    }
  }

  logFile.close();
}

bool SetSetting(const char* key, int value) {  //---------------------------- sttings save
  String newValue = (value == HIGH) ? "HIGH" : "LOW";
  String content = "";
  bool found = false;

  File file = SD.open("/settings.txt", FILE_READ);
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      line.trim();

      if (line.startsWith(key)) {
        content += String(key) + "=" + newValue + "\n";
        found = true;
      } else {
        content += line + "\n";
      }
    }
    file.close();
  }

  if (!found) {
    content += String(key) + "=" + newValue + "\n";
  }

  File writeFile = SD.open("/settings.txt", FILE_WRITE);
  if (!writeFile) return false;
  writeFile.print(content);
  writeFile.close();

  return true;
}

int GetSettings(const char* key) {  // ----------------------------------- Read settings
  File file = SD.open("/settings.txt", FILE_READ);
  if (!file) return LOW;  // Default

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.startsWith(key)) {
      int index = line.indexOf('=');
      if (index > 0) {
        String val = line.substring(index + 1);
        val.trim();

        file.close();
        return (val == "HIGH") ? HIGH : LOW;
      }
    }
  }

  file.close();
  return LOW;  // default if not found
}

void SendSettingsToCmd() {  // ------------------------------------------------- Read all the settings in once
  File file = SD.open("/settings.txt", FILE_READ);

  if (!file) {
    Blynk.virtualWrite(V2, "⚠ No settings.txt found");
    return;
  }

  Blynk.virtualWrite(V2, "📌 Current Settings:");

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.length() == 0) continue;

    int idx = line.indexOf('=');
    if (idx > 0) {
      String key = line.substring(0, idx);
      String value = line.substring(idx + 1);

      Blynk.virtualWrite(V2, key + " = " + value);
      delay(150);
    }
  }

  file.close();
}


//----------------------------------------------------------------------------- SD Card END ----------------------------------------------------------------------------------------
void irSwitch() {

  if (irSensor == "i") {
    if (flag5) {
      pcf1.digitalWrite(RF_CH, HIGH);
      pcf1.digitalWrite(rf3, LOW);
      delay(1000);
      pcf1.digitalWrite(RF_CH, LOW);
      flag5 = false;
    }
  }

  if (irSensor == "o") {
    if (!flag5) {
      pcf1.digitalWrite(RF_CH, HIGH);
      pcf1.digitalWrite(rf3, HIGH);
      delay(1000);
      pcf1.digitalWrite(RF_CH, LOW);
      flag5 = true;
    }
  }
}


void Serial_Read() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();

    int commaIndex1 = data.indexOf(',');
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = data.indexOf(',', commaIndex2 + 1);
    int commaIndex4 = data.indexOf(',', commaIndex3 + 1);
    int commaIndex5 = data.indexOf(',', commaIndex4 + 1);
    int commaIndex6 = data.indexOf(',', commaIndex5 + 1);

    if (commaIndex1 != -1 && commaIndex2 != -1 && commaIndex3 != -1 && commaIndex4 != -1 && commaIndex5 != -1 && commaIndex6 != -1) {

      String value1 = data.substring(0, commaIndex1);
      String value2 = data.substring(commaIndex1 + 1, commaIndex2);
      String value3 = data.substring(commaIndex2 + 1, commaIndex3);
      String value4 = data.substring(commaIndex3 + 1, commaIndex4);
      String value5 = data.substring(commaIndex4 + 1, commaIndex5);
      String value6 = data.substring(commaIndex5 + 1, commaIndex6);
      String value7 = data.substring(commaIndex6 + 1);

      VoltSensor = value1.toFloat();
      AmpSensor = value2.toFloat();
      microControllVolt_analog = value3.toInt();
      batteryVoltage_main = value4.toFloat();
      powerW = value5.toFloat();
      irSensor = value7;

      if (thunder) {
        rainSensor = value6.toInt();
      } else {
        rainSensor = 1024;
        rainCounter = 0;
        Blynk.virtualWrite(V2, "lightning protection off!");
        Blynk.virtualWrite(V2, value6.toInt());
      }
    }

    batteryVoltage_sys = microControllVolt_analog * (maxSystemBatteryVoltage / maxADCValue);
  }
}

void readTemp() {
  if (millis() - lastDHTReadTime >= dhtInterval) {
    lastDHTReadTime = millis();  // Update the last read time

    float temp1 = dht2.readTemperature();
    delay(10);
    float temp2 = dht3.readTemperature();
    delay(10);
    float temp3 = dht.readTemperature();
    envH = dht.readHumidity();
    delay(10);

    if (dhtchk > 10) {
      Blynk.virtualWrite(V2, "System temp sensor failed!");
    }

    if (dhtchk1 > 10) {
      Blynk.virtualWrite(V2, "Battery temp sensor failed!");
    }

    if (dhtchk2 > 10) {
      Blynk.virtualWrite(V2, "Environment temp sensor failed!");
    }

    // Apply filters
    if (!isnan(temp1) && temp1 >= 10 && temp1 <= 70) {
      systemTemp = temp1;
      dhtchk = 0;
    } else {
      dhtchk++;
    }

    if (!isnan(temp2) && temp2 >= 10 && temp2 <= 70) {
      batteryTemp = temp2;
      dhtchk1 = 0;
    } else {
      dhtchk1++;
    }

    if (!isnan(temp3) && temp3 >= 10 && temp3 <= 70) {
      envT = temp3;
      dhtchk2 = 0;
    } else {
      dhtchk2++;
    }
  }
}

void midNightAutoLights() {  // automatic turn on lights when house main power is off at 10PM to 5AM (only when inside pir detects someone)

  bool pirState = digitalRead(insidePir);

  if (Hours >= 22 || Hours <= 4) {
    flag6 = true;

    if (pirState && !pirPreviouslyDetected) {
      if (!toggleState) {
        pirCounter = 2000;
      } else {
        pirCounter = 0;
      }
      toggleState = !toggleState;
      pirPreviouslyDetected = true;
    }

    else if (!pirState) {
      pirPreviouslyDetected = false;
    }

    if (pirCounter > 1) {
      rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  //green off
      rgbLed.show();
      ledcWrite(kitchenCh, 200);
      Blynk.virtualWrite(V7, 250);
      Blynk.virtualWrite(V2, pirCounter);
    } else {

      if (xX) {
        rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 10));  // blue on
      } else {
        rgbLed.setPixelColor(0, rgbLed.Color(0, 10, 0));  // green on
      }
      rgbLed.show();
      ledcWrite(kitchenCh, 0);
      Blynk.virtualWrite(V7, 0);
      pirCounter = 0;
    }

    if (VoltSensor < 210) {

      if (pirCounter > 0) {
        if (!flag3) {
          ledcWrite(stairsCh, 50);
          ledcWrite(livingCh, 0);
          ledcWrite(diningCh, 50);
          ledcWrite(kitchenCh, 200);

          Blynk.virtualWrite(V4, 50);
          Blynk.virtualWrite(V6, 50);
          Blynk.virtualWrite(V7, 200);

          flag3 = true;
        }
      }

      else {

        if (flag3) {
          ledcWrite(stairsCh, 0);
          ledcWrite(diningCh, 0);
          ledcWrite(kitchenCh, 0);
          Blynk.virtualWrite(V4, 0);
          Blynk.virtualWrite(V6, 0);
          Blynk.virtualWrite(V7, 0);

          flag3 = false;
        }
      }
      flag7 = true;
    }

    else {
      if (flag7) {
        ledcWrite(stairsCh, 0);
        ledcWrite(diningCh, 0);
        ledcWrite(kitchenCh, 0);
        Blynk.virtualWrite(V4, 0);
        Blynk.virtualWrite(V6, 0);
        Blynk.virtualWrite(V7, 0);
        flag7 = false;
      }
    }
  }  //---------------------------------------------

  else {
    if (flag6) {
      LedAllOff();
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V6, 0);
      Blynk.virtualWrite(V7, 0);
      flag6 = false;
    }
  }


  if (pirCounter > 0) {
    pirCounter--;
  }
}



void autoLight() {

  if (Hours >= 18 && Hours <= 2 && VoltSensor < 210) {
    if (!autolightTriggered) {

      autolightEnable = true;
      autolightTriggered = true;
      Blynk.virtualWrite(V2, "Auto Lights ON");

      ledcWrite(KLroomCh, 0);
      ledcWrite(stairsCh, 50);
      ledcWrite(livingCh, 0);
      ledcWrite(diningCh, 100);
      ledcWrite(kitchenCh, 200);
      ////////////////////////
      Blynk.virtualWrite(V4, 50);
      Blynk.virtualWrite(V6, 100);
      Blynk.virtualWrite(V7, 200);
    }
  } else {
    if (autolightEnable) {

      autolightEnable = false;
      autolightTriggered = false;
      Blynk.virtualWrite(V2, "Auto Lights OFF");
      LedAllOff();
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V6, 0);
      Blynk.virtualWrite(V7, 0);
    }
  }
}
void saftySys() {  // checking all the temp sensors and voltage sensors if any problems send notification and cmd lines
  float avaTemp = envT + 2.5;

  if (systemTemp > avaTemp) {
    pcf1.digitalWrite(fan, HIGH);
  } else {
    pcf1.digitalWrite(fan, LOW);
  }

  avaTemp = 0;
  //////////////////////////////////////////////////////////////////////////////////////////
  if (VoltSensor > 265) {
    if (voltHighCount < 10) {
      voltHighCount++;
    }

    if (voltHighCount >= 8) {
      if (!sFlag1) {
        pcf1.digitalWrite(pwrCut, LOW);
        delay(1000);
        pcf1.digitalWrite(pwrCut, HIGH);
        delay(500);
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main 230V is too high!");
        sFlag1 = true;
      }
      Blynk.virtualWrite(V2, "Main 230V is too high! Count: " + String(voltHighCount));
      addError("Main 230V is too high!");
    }
  } else {
    if (sFlag1) {
      voltHighCount = 0;
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "Main 230V Back in Stable");
      sFlag1 = false;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if (VoltSensor > 90 && VoltSensor < 200) {

    if (voltLowCount < 10) {
      voltLowCount++;
    }

    if (voltLowCount >= 8) {
      if (!sFlag2) {
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main 230V is too low!");
        sFlag2 = true;
      }
      Blynk.virtualWrite(V2, "Main 230V is too Low! Count: " + String(voltLowCount));
      addError("Main 230V is too low!");
    }
  }

  else {
    if (sFlag2) {
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "Main 230V Back in Stable");
      voltLowCount = 0;
      sFlag2 = false;
    }
  }
  //////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_main > 17) {
    if (batteryMainHighCount < 10) {
      batteryMainHighCount++;
    }

    if (batteryMainHighCount >= 6) {
      if (!sFlag3) {
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "Main battery is overcharging!");
        LedAllOff();
        sFlag3 = true;
      }
      Blynk.virtualWrite(V2, "Main battery is overcharging! Count: " + String(batteryMainHighCount));
      addError("Main battery is overcharging!");
    }

  } else {
    if (sFlag3) {
      batteryMainHighCount = 0;
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "Main battery voltage is now stable");
      sFlag3 = false;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_main < 12) {
    if (batteryMainLowCount < 10) {
      batteryMainLowCount++;
    }

    if (batteryMainLowCount >= 5) {
      if (!sFlag4) {
        Blynk.logEvent("volts_amps_sensors", "Main battery voltage is too low!");
        LedAllOff();
        sFlag4 = true;
      }
      Blynk.virtualWrite(V2, "Main battery voltage is too low! Count: " + String(batteryMainLowCount));
      addError("Main battery voltage is too low!");
    }
  } else {
    if (sFlag4) {
      Blynk.logEvent("volts_amps_sensors", "Main battery voltage is now stable");
      batteryMainLowCount = 0;
      sFlag4 = false;
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_sys > 6) {
    if (batterySysHighCount < 10) {
      batterySysHighCount++;
    }
    if (batterySysHighCount >= 5) {
      if (!sFlag5) {
        pcf1.digitalWrite(lightninProtect, LOW);
        Blynk.logEvent("volts_amps_sensors", "System voltage is too high!");
        LedAllOff();
        sFlag5 = true;
      }
      Blynk.virtualWrite(V2, "System voltage is too high! Count: " + String(batterySysHighCount));
      addError("System voltage is too high!");
    }
  } else {
    if (sFlag5) {
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.logEvent("volts_amps_sensors", "System voltage now stable");
      batterySysHighCount = 0;
      sFlag5 = false;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////

  if (batteryVoltage_sys < 3.2) {
    if (batterySysLowCount < 10) {
      batterySysLowCount++;
    }
    if (batterySysLowCount >= 5) {
      if (!sFlag6) {
        Blynk.logEvent("volts_amps_sensors", "System voltage is too low!");
        sFlag6 = true;
      }
      Blynk.virtualWrite(V2, "System voltage is too low! Count: " + String(batterySysLowCount));
      addError("System voltage is too low!");
    }
  } else {
    if (sFlag6) {
      Blynk.logEvent("volts_amps_sensors", "System voltage is now stable");
      batterySysLowCount = 0;
      sFlag6 = false;
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////

  if (systemTemp > 39) {
    if (systemTempCount < 10) {
      systemTempCount++;
    }
    if (systemTempCount >= 5) {
      if (!sFlag7) {
        Blynk.logEvent("temp", "System Temp is too high!");
        LedAllOff();
        sFlag7 = true;
      }
      Blynk.virtualWrite(V2, "System Temp is too high! Count: " + String(systemTempCount));
      addError("System Temp is too high!");
    }
  } else {
    if (sFlag7) {
      Blynk.logEvent("temp", "System Temp is back in normal");
      systemTempCount = 0;
      sFlag7 = false;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////

  if (batteryTemp > 36) {
    if (batteryTempCount < 10) {
      batteryTempCount++;
    }
    if (batteryTempCount >= 5) {
      if (!sFlag8) {
        Blynk.logEvent("temp", "Battery Temp is too high!");
        LedAllOff();
        sFlag8 = true;
      }
      Blynk.virtualWrite(V2, "Battery Temp is too high! Count: " + String(batteryTempCount));
      addError("Battery Temp is too high!");
    }
  } else {
    if (sFlag7) {
      Blynk.logEvent("temp", "Battery Temp back in normal");
      batteryTempCount = 0;
      sFlag7 = false;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////

  if (envT > 37) {
    if (envTCount < 10) {
      envTCount++;
    }
    if (envTCount >= 6) {
      if (!sFlag9) {
        Blynk.logEvent("temp", "Power Supply Temp is too high!");
        LedAllOff();
        sFlag9 = true;
      }
      Blynk.virtualWrite(V2, "Power Supply Temp is too high! Count: " + String(envTCount));
      addError("Power Supply Temp is too high!");
    }
  } else {
    if (sFlag9) {
      Blynk.logEvent("temp", "Power Supply Temp is back in normal");
      envTCount = 0;
      sFlag9 = false;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////
  if (systemTemp > 40 || batteryTemp > 45 || envT > 40) {
    if (fireDetectionCount < 20) {
      fireDetectionCount++;
    }
    if (fireDetectionCount >= 18) {
      if (!sFlag10) {
        Blynk.logEvent("temp", "🔥 Fire Detected!!!");
        pcf1.digitalWrite(pwrCut, LOW);
        delay(1000);
        pcf1.digitalWrite(pwrCut, HIGH);
        LedAllOff();
        sFlag10 = true;
      }
      Blynk.virtualWrite(V2, "🔥 Fire Detected!!! Count: " + String(fireDetectionCount));
      addError("Fire Detected!!!");
    }
  } else {
    if (sFlag10) {
      fireDetectionCount = 0;
      sFlag10 = false;
    }
  }
  ////////////////////////////////////////////////Lightning protect on/////////////////////////////////////////////////////

  //lightnin Protect
  if (rainSensor < 1018) {
    rainDetectCount++;
    if (rainDetectCount > 10) {
      rainCounter = 300;
      Blynk.logEvent("env", "Rain Detected!!");
    }
  }

  else {
    rainDetectCount = 0;  // Reset if no rain detected
  }

  if (rainCounter > 0) {
    pcf1.digitalWrite(lightninProtect, LOW);
    Blynk.virtualWrite(V2, "Lightning protect on");
    logRain = String(timestamp) + ", Raining";
    rainProtect = true;
    rainCounter--;
  }

  else {
    if (rainProtect) {
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.virtualWrite(V2, "Lightning protect off");
      logRain = String(timestamp) + ", No Rain";
      rainProtect = false;
    }
  }
}

void autoSecurty() {               // pir sensors at out side automatic work and notifications
  if (Hours >= 18 || Hours < 6) {  // 6PM to 6AM
    if (digitalRead(outsidePir_top)) {
      Blynk.logEvent("motion_detected", "Motion detected top left outside!");
    }
  }
}

void Securty_mode() {  // activate with button, all pir's working at same time
  if (SecuMode) {

    if (digitalRead(insidePir)) {
      pirCount1 = 50;
      Blynk.logEvent("motion_detected", "Motion detected inside of the house!");
    }

    if (digitalRead(outsidePir_top)) {
      Blynk.logEvent("motion_detected", "Motion detected top left outside!");
    }



    if (pirCount1 > 0) {

      if (!alarmCutOff) {
        pcf1.digitalWrite(alarm, HIGH);
      }

      Blynk.virtualWrite(V2, pirCount1);

      ledcWrite(KLroomCh, 220);
      ledcWrite(stairsCh, 220);
      ledcWrite(livingCh, 220);
      ledcWrite(diningCh, 220);
      ledcWrite(kitchenCh, 220);

      pcf1.digitalWrite(rf1, LOW);
      pcf1.digitalWrite(rf2, LOW);
      pcf1.digitalWrite(rf3, LOW);
      pcf1.digitalWrite(rf4, LOW);
      pcf1.digitalWrite(RF_CH, HIGH);

      delay(random(50, 150));

      LedAllOff();
      pcf1.digitalWrite(RF_CH, LOW);
      Blynk.virtualWrite(V2, "All RF lights ON");

      delay(random(50, 150));

      pirCount1--;

    }

    else {
      LedAllOff();
      pcf1.digitalWrite(alarm, LOW);
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
  }
}

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


BLYNK_WRITE(V3) {
  if (param.asInt()) {
    SecuMode = true;
    Blynk.virtualWrite(V2, "Securty Mode is on");
    if (alarmCutOff) {
      Blynk.virtualWrite(V2, "Alarm is Locked! (Alarm Off)");
    }
  } else {
    SecuMode = false;
    Blynk.virtualWrite(V2, "Securty Mode is off");
    alarmCutOff = true;
    pcf1.digitalWrite(alarm, LOW);
    pcf1.digitalWrite(alarm, LOW);
    pcf1.digitalWrite(rf1, HIGH);
    pcf1.digitalWrite(rf2, HIGH);
    pcf1.digitalWrite(rf3, HIGH);
    pcf1.digitalWrite(rf4, HIGH);
    delay(500);
    pcf1.digitalWrite(RF_CH, HIGH);
    delay(500);
    pcf1.digitalWrite(RF_CH, LOW);
    Blynk.virtualWrite(V2, "All RF lights OFF");

    pirCount1 = 0;

    LedAllOff();
  }
}

BLYNK_WRITE(V4) {  // stairs
  int val = param.asInt();
  ledcWrite(stairsCh, val);
}

BLYNK_WRITE(V1) {
  int val = param.asInt();  // KLroom
  ledcWrite(KLroomCh, val);
}

BLYNK_WRITE(V5) {
  int val = param.asInt();
  ledcWrite(livingCh, val);  // Living
}

BLYNK_WRITE(V6) {
  int val = param.asInt();
  ledcWrite(diningCh, val);  // Dining
}

BLYNK_WRITE(V7) {
  int val = param.asInt();
  ledcWrite(kitchenCh, val);  // Kitchen
}

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

void timeDateUpdate() {
  timeClient.update();
  Hours = timeClient.getHours();
  MiN = timeClient.getMinutes();
  sec = timeClient.getSeconds();

  unsigned long epochTime = timeClient.getEpochTime();
  int currentYear = year(epochTime);
  int currentMonth = month(epochTime);
  int currentDay = day(epochTime);

  sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d",
          currentYear, currentMonth, currentDay,
          Hours, MiN, sec);

  // ---------- Daily SD space check ----------
  if (Hours == 0 && MiN == 0) {  // At 00:00 midnight
    if (currentDay != lastCheckedDay) {
      checkCardSpace();
      lastCheckedDay = currentDay;
    }
  }
}

BLYNK_WRITE(V2) {  // ----------------------------------------------------------------------------------------- cmd command reading -----------------------
  cmd = param.asStr();

  if (cmd.startsWith("#")) {
    Serial.print(cmd);
  }

  if (cmd == "pCut") {
    pcf1.digitalWrite(pwrCut, LOW);
    delay(1000);
    pcf1.digitalWrite(pwrCut, HIGH);
    Serial_Read();
    delay(1000);
    if (VoltSensor < 30) {
      Blynk.virtualWrite(V2, "Main Power is off!");
    } else {
      Blynk.virtualWrite(V2, "Power Cut-off is faild!");
    }
  }

  if (cmd == "test") {
    testV2cmd = true;
  } else {
    testV2cmd = false;
  }

  if (cmd == "sftyOn") {
    safetyMode = true;
  }
  if (cmd == "sftyOff") {
    safetyMode = false;
  }

  if (cmd == "getAmps") {
    cmd_systemAmp = true;
  } else {
    cmd_systemAmp = false;
  }

  if (cmd == "getRain") {
    cmd_rainSensor = true;
  } else {
    cmd_rainSensor = false;
  }

  if (cmd == "getSystemVolt") {
    cmd_systemVolt = true;
  } else {
    cmd_systemVolt = false;
  }

  if (cmd == "getSystemTemp") {
    cmd_sysTemp = true;
  } else {
    cmd_sysTemp = false;
  }

  if (cmd == "getBatteryTemp") {
    cmd_batTemp = true;
  } else {
    cmd_batTemp = false;
  }

  if (cmd == "testInPir") {
    cmd_testPir1 = true;
  } else {
    cmd_testPir1 = false;
  }

  if (cmd == "rstCount") {
    cmd_rstCount = true;
  } else {
    cmd_rstCount = false;
  }

  if (cmd == "testOutTopPir") {
    cmd_testPir2 = true;
  } else {
    cmd_testPir2 = false;
  }

  if (cmd == "offRain") {
    Blynk.virtualWrite(V2, "Automatic lightning protection off!");
    thunder = false;
  }

  if (cmd == "onRain") {
    Blynk.virtualWrite(V2, "Automatic lightning protection on!");
    thunder = true;
  }

  if (cmd == "ELightOff") {
    pirCounter = 0;
    LedAllOff();
  }

  if (cmd == "Xauto") {
    if (SetSetting("Xmode", HIGH)) {
      Blynk.virtualWrite(V2, "New settings saved!");
    } else {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "Xmanual") {
    if (SetSetting("Xmode", LOW)) {
      Blynk.virtualWrite(V2, "New settings saved!");
    } else {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "MidNightAutoLightOn") {
    if (SetSetting("m_AutoLight", HIGH)) {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_m_autoLight = true;
    } else {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "MidNightAutoLightOff") {
    if (SetSetting("m_AutoLight", LOW)) {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_m_autoLight = false;
    } else {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "AutoLightOn") {
    if (SetSetting("AutoLight", HIGH)) {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_autoLight = true;
    } else {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "AutoLightOff") {
    if (SetSetting("AutoLight", LOW)) {
      Blynk.virtualWrite(V2, "New settings saved!");
      Local_autoLight = false;
    } else {
      Blynk.virtualWrite(V2, "Failed to save new settings");
    }
  }

  if (cmd == "readSettings") {
    SendSettingsToCmd();
  }


  if (cmd == "chkSD") {
    checkCardSpace();
  }

  if (cmd == "sftyLog") {
    sendLastLinesFromFile("/Safety_system_Error_log.csv", "Error Log");
    sendLastLinesFromFile("/Safety_system_Status_log.csv", "Status Log");
  }

  if (cmd == "firmLog") {
    sendFirmwareLog();
  }

  if (cmd == "autoLightOff") {
    if (pirCounter > 0) {
      Blynk.virtualWrite(V2, pirCounter);
      pirCounter = 0;
      Blynk.virtualWrite(V2, "Auto emergency light off!");
    } else {
      Blynk.virtualWrite(V2, "No movement detect!");
    }
  }

  if (cmd == "about") {
    sendAboutInfo();
  }

  if (cmd == "almLocOn") {
    alarmCutOff = true;
    pcf1.digitalWrite(alarm, LOW);
    Blynk.virtualWrite(V2, "Alarm Locked!");
  }

  if (cmd == "almLocOff") {
    alarmCutOff = false;
    Blynk.virtualWrite(V2, "Alarm Unlocked!");
  }

  if (cmd == "almOn") {
    if (alarmCutOff) {
      Blynk.virtualWrite(V2, "Alarm is Locked");
    } else {
      pcf1.digitalWrite(alarm, HIGH);
      Blynk.virtualWrite(V2, "Alarm is on");
    }
  }

  if (cmd == "almOff") {
    pcf1.digitalWrite(alarm, LOW);
    Blynk.virtualWrite(V2, "Alarm is off");
  }

  if (cmd == "pwrGon") {
    pcf1.digitalWrite(6, LOW);
    delay(100);
    if (pcf1.digitalRead(6) == LOW) {
      Blynk.virtualWrite(V2, "Lightning protection on");
    } else {
      Blynk.virtualWrite(V2, "Lightning protection failed!");
    }
  }

  if (cmd == "pwrGoff") {
    pcf1.digitalWrite(6, HIGH);
    delay(100);
    if (pcf1.digitalRead(6) == HIGH) {
      Blynk.virtualWrite(V2, "Lightning protection off");
    } else {
      Blynk.virtualWrite(V2, "Lightning protection failed!");
    }
  }

  if (cmd == "fan") {
    pcf1.digitalWrite(fan, HIGH);
    delay(5000);
    pcf1.digitalWrite(fan, LOW);
    Blynk.virtualWrite(V2, "Fan Checked!");
  }

  if (cmd == "reset") {
    Blynk.virtualWrite(V2, "Restarting ESP32...");
    delay(1000);
    esp_restart();
  }
  if (cmd == "rf1On") {
    pcf1.digitalWrite(rf1, LOW);
    Blynk.virtualWrite(V2, "RF SW1 ON");
  }
  if (cmd == "rf1Off") {
    pcf1.digitalWrite(rf1, HIGH);
    Blynk.virtualWrite(V2, "RF SW1 OFF");
  }

  if (cmd == "rf2On") {
    pcf1.digitalWrite(rf2, LOW);
    Blynk.virtualWrite(V2, "RF SW2 ON");
  }
  if (cmd == "rf2Off") {
    pcf1.digitalWrite(rf2, HIGH);
    Blynk.virtualWrite(V2, "RF SW2 OFF");
  }

  if (cmd == "rf3On") {
    pcf1.digitalWrite(rf3, LOW);
    Blynk.virtualWrite(V2, "RF SW3 ON");
  }
  if (cmd == "rf3Off") {
    pcf1.digitalWrite(rf3, HIGH);
    Blynk.virtualWrite(V2, "RF SW3 OFF");
  }

  if (cmd == "rf4On") {
    pcf1.digitalWrite(rf4, LOW);
    Blynk.virtualWrite(V2, "RF SW4 ON");
  }
  if (cmd == "rf4Off") {
    pcf1.digitalWrite(rf4, HIGH);
    Blynk.virtualWrite(V2, "RF SW4 OFF");
  }

  if (cmd == "rfCH1") {
    pcf1.digitalWrite(RF_CH, HIGH);
    Blynk.virtualWrite(V2, "RF Channel 1");
  }

  if (cmd == "rfCH2") {
    pcf1.digitalWrite(RF_CH, LOW);
    Blynk.virtualWrite(V2, "RF Channel 2");
  }

  if (cmd == "rfAllOff") {
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

  if (cmd == "rfAllOn") {
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

  if (cmd == "rfStatus") {

    Blynk.virtualWrite(V2, "RF Status Report");
    delay(100);

    if (pcf1.digitalRead(RF_CH)) {  //////////////////////////// rf ch 1
      Blynk.virtualWrite(V2, "RF Channel 1 (Ground Floor)");
      delay(100);

      if (pcf1.digitalRead(rf1)) {
        Blynk.virtualWrite(V2, "RF SW1 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW1 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf2)) {
        Blynk.virtualWrite(V2, "RF SW2 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW2 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf3)) {
        Blynk.virtualWrite(V2, "RF SW3 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW3 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf4)) {
        Blynk.virtualWrite(V2, "RF SW4 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW4 ON");
      }
    }

    else {  //////////////////////////////////////////////// rf ch 2
      Blynk.virtualWrite(V2, "RF Channel 2 (Mid Floor)");
      delay(100);

      if (pcf1.digitalRead(rf1)) {
        Blynk.virtualWrite(V2, "RF SW1 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW1 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf2)) {
        Blynk.virtualWrite(V2, "RF SW2 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW2 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf3)) {
        Blynk.virtualWrite(V2, "RF SW3 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW3 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf4)) {
        Blynk.virtualWrite(V2, "RF SW4 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW4 ON");
      }
    }
  }


  if (cmd == "Xon") {
    if (GetSettings("Xmode")) {
      Blynk.virtualWrite(V2, "Auto X mode is on!");
    }
    xX = true;
    flag1 = true;
  }
  if (cmd == "Xoff") {
    xX = false;
    flag1 = true;
  }

  if (cmd == "help") {
    Blynk.virtualWrite(V2, "📖 Command Help Menu:");
    delay(100);

    Blynk.virtualWrite(V2, "pCut - Cut main power temporarily");
    Blynk.virtualWrite(V2, "test - Enable test mode");
    Blynk.virtualWrite(V2, "sftyOn / sftyOff - Safety mode ON/OFF");
    Blynk.virtualWrite(V2, "getAmps - Get system current");
    Blynk.virtualWrite(V2, "getRain - Get rain sensor status");
    Blynk.virtualWrite(V2, "getSystemVolt - Get system voltage");
    Blynk.virtualWrite(V2, "getSystemTemp - Get system temperature");
    Blynk.virtualWrite(V2, "getBatteryTemp - Get battery temperature");
    Blynk.virtualWrite(V2, "testInPir / testOutTopPir - PIR test modes");
    Blynk.virtualWrite(V2, "rstCount - Reset counter");
    Blynk.virtualWrite(V2, "ELightOff - Emergency lights OFF");
    Blynk.virtualWrite(V2, "chkSD - Check SD card space");
    Blynk.virtualWrite(V2, "sftyLog - Send safety logs");
    Blynk.virtualWrite(V2, "firmLog - Send firmware log");
    Blynk.virtualWrite(V2, "autoLightOff - Force auto light OFF");
    Blynk.virtualWrite(V2, "about - Show about info");
    Blynk.virtualWrite(V2, "almLocOn / almLocOff - Lock/Unlock alarm");
    Blynk.virtualWrite(V2, "almOn / almOff - Turn alarm ON/OFF");
    Blynk.virtualWrite(V2, "pwrGon / pwrGoff - Lightning protection ON/OFF");
    Blynk.virtualWrite(V2, "fan - Test fan");
    Blynk.virtualWrite(V2, "reset - Restart ESP32");
    Blynk.virtualWrite(V2, "rf1On / rf1Off ... rf4On / rf4Off - RF Switch control");
    Blynk.virtualWrite(V2, "rfCH1 / rfCH2 - Select RF Channel");
    Blynk.virtualWrite(V2, "rfAllOn / rfAllOff - RF lights ON/OFF");
    Blynk.virtualWrite(V2, "rfStatus - Get RF status report");
  }
}
