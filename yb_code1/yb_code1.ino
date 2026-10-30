//=================================================
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "TFT9341Touch.h"
#include <WiFi.h>
#include <WebServer.h>

/* -------- Pin Definitions -------- */
int INA = 26;                 // Motor control pin A
int INB = 25;                 // Motor control pin B
#define DHTPIN 13              // DHT22 data pin
#define DHTTYPE DHT22
int waterSensorPin = 14;      // Water presence sensor
int ldrPin = 32;              // Light sensor (ADC1 pin)
int waterLevelSensorPin = 34; // Water level analog sensor

/* -------- Sensor Variables -------- */
int waterLevelValue = 0;
int lightValue = 0;
bool waterValue = 0;
float currentTemp = 0;
float currentHum = 0;

/* -------- WiFi Credentials -------- */
const char* ssid = "Melton-Teachers";
const char* password = "Melton-2018";

/* -------- Objects -------- */
tft9341touch lcd(5, 4, 15, 35); 
WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

/* -------- Timing Control -------- */
unsigned long lastSensorRead = 0;
const long sensorInterval = 2000;

/* -------- Touch Button Settings -------- */
bool displayMode = false;
int buttonX = 60;
int buttonY = 100;
int buttonW = 200;
int buttonH = 50;

void setup() {
  Serial.begin(9600);
  delay(100);

  // Configure ADC resolution and range
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  // Initialize display and touch screen
  lcd.begin();
  lcd.setTouch(3780, 372, 489, 3811);
  lcd.setTextSize(1);

  // Configure pin modes
  pinMode(waterLevelSensorPin, INPUT);
  pinMode(waterSensorPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);

  // Initialize system components
  connectToWifi();
  setupServer();
  server.begin();
  dht.begin();

  // Draw the on-screen button
  drawButton();

  Serial.println("System initialized successfully");
}

/* -------- Draw Touch Button -------- */
void drawButton() {
  lcd.fillRoundRect(buttonX, buttonY, buttonW, buttonH, 5, GREEN);
  lcd.print(buttonX + 20, buttonY + 15, "Change Display", 2, BLACK, GREEN);
}

/* -------- Update Screen Content -------- */
void updateDisplay() {
  lcd.fillRect(0, 0, 320, buttonY - 10, BLACK);

  if (displayMode) {
    lcd.print(20, 20, "Welcome to my", 2, WHITE, BLACK);
    lcd.print(40, 45, "Greenhouse!", 3, GREEN, BLACK);
  } else {
    lcd.print(10, 20, "IP Address:", 2, WHITE, BLACK);
    lcd.print(10, 45, WiFi.localIP().toString(), 2, CYAN, BLACK);
  }
}

void loop() {
  // Handle incoming web requests
  server.handleClient();

  // Check for touch interaction
  if (lcd.touched()) {
    delay(30);
    lcd.readTouch();

    int x = lcd.xTouch;
    int y = lcd.yTouch;

    // Detect button press
    if (x > buttonX && x < (buttonX + buttonW) &&
        y > buttonY && y < (buttonY + buttonH)) {
      displayMode = !displayMode;
      updateDisplay();
    }

    // Wait until touch is released
    while (lcd.touched());
    delay(10);
  }

  // Read sensors at fixed intervals
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorRead >= sensorInterval) {
    lastSensorRead = currentMillis;

    lightValue = analogRead(ldrPin);
    currentTemp = dht.readTemperature();
    currentHum = dht.readHumidity();
    waterValue = digitalRead(waterSensorPin);
    waterLevelValue = analogRead(waterLevelSensorPin);

    // Automatic motor control based on temperature
    if (!isnan(currentTemp)) {
      if (currentTemp > 25) {
        digitalWrite(INA, LOW);
        digitalWrite(INB, HIGH);
      } else {
        digitalWrite(INA, LOW);
        digitalWrite(INB, LOW);
      }
    }

    // Serial monitoring for debugging
    Serial.print("Temp: ");
    Serial.print(currentTemp);
    Serial.print(" | Humidity: ");
    Serial.print(currentHum);
    Serial.print(" | Water Detected: ");
    Serial.print(!waterValue);
    Serial.print(" | Light Level: ");
    Serial.print(lightValue);
    Serial.print(" | Water Level: ");
    Serial.println(waterLevelValue);
  }
}
//=================================================