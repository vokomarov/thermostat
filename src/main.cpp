#include <HomeSpan.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include "secrets.h"
#include "RELAY.h"

#define RELAY_PIN GPIO_NUM_5
#define MANUAL_SW_PIN GPIO_NUM_10
#define LED_PIN GPIO_NUM_7

bool isRelayOverridden = false;
void manualBtnPoll();
unsigned long lastCheckedTime = 0;
const long interval = 1000; // 1 second

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MANUAL_SW_PIN, INPUT_PULLDOWN);
  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(115200);

  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G);

  homeSpan.setPairingCode("11122333");
  homeSpan.begin(Category::Switches, "Gas Boiler Switch");

  homeSpan.setWifiCredentials(WIFI_SSID, WIFI_PASSWORD);
  homeSpan.setStatusPin(GPIO_NUM_8);

  // Accessory: Relay Switch
  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify();                       
    new RELAY(RELAY_PIN, LED_PIN, MANUAL_SW_PIN);
}

void loop() {
  homeSpan.poll();

  unsigned long currentTime = millis();
  if (currentTime - lastCheckedTime >= interval) {
    lastCheckedTime = currentTime;
    manualBtnPoll();
  }
}

void manualBtnPoll() {
  boolean isManualBtnEnabled = digitalRead(MANUAL_SW_PIN) == HIGH;
  boolean isRelayEnabled = digitalRead(RELAY_PIN) == LOW;

  if (!isRelayOverridden && isManualBtnEnabled) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(RELAY_PIN, LOW);
    isRelayOverridden = true;
    Serial.println("Relay manually overridden to ON");
  } else if (!isManualBtnEnabled && isRelayOverridden && isRelayEnabled) {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY_PIN, HIGH);
    isRelayOverridden = false;
    Serial.println("Relay manually overridden to OFF");
  }
}
