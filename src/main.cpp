#include <HomeSpan.h>
#include "secrets.h"
#include "RELAY.h"

void setup() {
  Serial.begin(115200);

  homeSpan.setPairingCode("11122333");
  homeSpan.begin(Category::Switches, "Gas Boiler Switch");

  homeSpan.setWifiCredentials(WIFI_SSID, WIFI_PASSWORD);
  homeSpan.setStatusPin(GPIO_NUM_8);

  // Accessory: Relay Switch
  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify();                       
    new RELAY(GPIO_NUM_5);
}

void loop() {
  homeSpan.poll();
}
