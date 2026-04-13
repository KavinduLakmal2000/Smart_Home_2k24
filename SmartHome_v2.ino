//////////////////////////////////////////

// i2c Address :
// INA226 sensor - 0x44
// PCF8575 - 0x23
// OLED Display - ox3c

///////////////////////////////////////
const char* SoftVer = "Firmware 3.1.6";
const char* whatsNew = "gain control of top floor light";
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
#include <esp_task_wdt.h>
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

#define WDT_TIMEOUT 15 // 15 seconds watchdog timeout

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
bool cmd_time = false;

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
bool topLight = false;

const float maxSystemBatteryVoltage = 6.0;
const int maxADCValue = 1023;
float batteryVoltage_sys = 0;
float batteryVoltage_main = 0;
int rainSensor = 1023;
int rainThreshold = 1018;
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
bool sFlag13 = false;
bool sFlag14 = false;


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
bool Local_topLightAuto = false;

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
#define topLightPin 13        // pcf #white wire in 6pin connecter out 16v+
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
  // Initialize Watchdog Timer
  esp_task_wdt_init(WDT_TIMEOUT, true); // Enable panic so it reboots on timeout
  esp_task_wdt_add(NULL);               // Add current thread (loop task) to WDT

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
    esp_task_wdt_reset(); // Feed the dog during WiFi connection
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
    
    esp_task_wdt_reset(); // Feed the dog while waiting for hardware connection
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

  int savedRain = GetSettings("rain");
  if (savedRain > 0) {
    rainThreshold = savedRain;
  }
  Blynk.virtualWrite(V2, "Rain Limit: " + String(rainThreshold));
  delay(100);

  if (GetSettings("Xmode") == HIGH) {
    Blynk.virtualWrite(V2, "X on");
  } else {
    Blynk.virtualWrite(V2, "X off");
    xX = false;
  }
  delay(100);

  if (GetSettings("AutoLight") == HIGH) {
    Blynk.virtualWrite(V2, "Auto Lights on");
  } else {
    Blynk.virtualWrite(V2, "Auto Lights off");
    Local_autoLight = false;
  }
  delay(100);

  if (GetSettings("m_AutoLight") == HIGH) {
    Blynk.virtualWrite(V2, "Mid Night Auto Lights on");
  } else {
    Blynk.virtualWrite(V2, "Mid Night Auto Lights off");
    Local_m_autoLight = false;
  }

  delay(100);

  if (GetSettings("topLightAuto") == HIGH) {
    Blynk.virtualWrite(V2, "Top Light Schedule Enabled");
  } else {
    Blynk.virtualWrite(V2, "Top Light Schedule Disabled");
    Local_topLightAuto = false;
  }

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
      esp_task_wdt_reset(); // Feed the dog during animation
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
  pcf1.pinMode(topLightPin, OUTPUT);

  pcf1.digitalWrite(lightninProtect, HIGH);
  pcf1.digitalWrite(pwrCut, HIGH);
  pcf1.digitalWrite(fan, LOW);
  pcf1.digitalWrite(alarm, LOW);
  pcf1.digitalWrite(topLightPin, LOW);

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
    esp_task_wdt_reset(); // Feed the dog before the long final delay
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


void loop() {  //===================================================================================== loop start ===================================================================
  esp_task_wdt_reset(); // Reset the watchdog timer at the start of every loop

  timeDateUpdate(); // Sync NTP time and update Hours, MiN, and timestamp variables

  resetCounter++;


  Serial_Read();   // read promini serial data
  x_Mode();        // the room x mode
  Securty_mode();  // activate with button, all pir's working at same time, alarm lock off
  readTemp();      // read all the temp sensors and filter output values
                   //autoSecurty();                   // pir sensors at out side automatic work and notifications
  irSwitch();
  TopLight(); // control top floor light

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

  if (cmd_time){
    Blynk.virtualWrite(V2, "System Time: " + String(Hours) + ":" + String(MiN) + ":" + String(sec));
  }


  if (alarmCutOff) {
    pcf1.digitalWrite(alarm, LOW);
  }

  connectionCheck();

  delay(300);
}  // ================================================================================================= loop end ==================================================================


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
