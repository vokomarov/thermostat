#include <Arduino.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <Thread.h>
#include <secrets.h>
#include <config.h>
#include <ota.h>
#include <network.h>

#define LED_MODE_OFF 0
#define LED_MODE_STANDBY 1
#define LED_MODE_BLINKING 2

extern WiFiClientSecure client;
extern NTPClient timeClient;

Thread monitorThread = Thread();
Thread ledThread = Thread();
Thread timeSyncThread = Thread();

int ledMode = LED_MODE_OFF;
int ledState = -1;

void sensorThreadFunc();
void monitorThreadFunc();
void ledThreadFunc();

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.printf("Booting version %s (build %s, %s)\n\r", VERSION, __DATE__, __TIME__);
  Serial.printf("Connecting to WiFi %s\n\r", WIFI_SSID);

  initWiFi();

  // set boot LED animation to let serial connection happens for debugging
  for (int step = -255; step <= 255; step++) {
    analogWrite(LED_PIN, abs(abs(step)));
    delay(1);
  }

  initOTA();
  initTimeSync();

  Serial.println("Boot completed");

  // threads configuration
  monitorThread.onRun(monitorThreadFunc);
  monitorThread.setInterval(5000);

  ledThread.onRun(ledThreadFunc);
  ledThread.setInterval(100);

  timeSyncThread.onRun(doTimeSync);
  timeSyncThread.setInterval(1000 * 60 * 10); // 10 minutes
}

void loop() {
  checkOTA();
  checkWiFi();

  if (monitorThread.shouldRun()) {
    monitorThread.run();
  }

  if (ledThread.shouldRun()) {
    ledThread.run();
  }

  if (timeSyncThread.shouldRun()) {
    timeSyncThread.run();
  }
}

void monitorThreadFunc() {
  String timestamp = timeClient.getFormattedTime();
  char buff [512];

  sprintf(buff, "[%s][M] WiFi connected: %s, RSSI: %d\n\r", timestamp, WiFi.isConnected() ? "yes" : "no", WiFi.RSSI());
  
  Serial.print(buff);
}

#define LED_OFF_FREQ 1000
#define LED_STANDBY_FREQ 1000
#define LED_BLINKING_FREQ 100

unsigned long ledChangedStateMillis;

void ledThreadFunc() {
  unsigned long currentMillis = millis();
  bool shouldWrite = false;

  if (ledMode == LED_MODE_OFF && ledState != LOW) {
    shouldWrite = true;
    ledState = LOW;
  }

  if (ledMode == LED_MODE_STANDBY && currentMillis - ledChangedStateMillis >= LED_STANDBY_FREQ) {
    shouldWrite = true;
    ledState = not(ledState);
  }

  if (ledMode == LED_MODE_BLINKING && currentMillis - ledChangedStateMillis >= LED_BLINKING_FREQ) {
    shouldWrite = true;
    ledState = not(ledState);
  }

  if (!shouldWrite) {
    return;
  }

  analogWrite(LED_PIN, ledState == HIGH ? 0 : 255);
  ledChangedStateMillis = currentMillis;
}
