#include <HomeSpan.h>

struct RELAY : Service::Switch {

  int relayPin;
  int statusPin;
  int overrideBtnPin;
  SpanCharacteristic *power;

  RELAY(int relayPin, int statusPin, int overrideBtnPin) : Service::Switch() 
  {
    power = new Characteristic::On(HIGH, true);
    this->relayPin = relayPin;
    this->statusPin = statusPin;
    this->overrideBtnPin = overrideBtnPin;
    digitalWrite(this->relayPin, HIGH); // turn off relay's Normally Open pad
  }

  boolean update() 
  {
    boolean isStateOverridden = digitalRead(overrideBtnPin) == HIGH;
    
    if (isStateOverridden) {
      return true;
    }

    digitalWrite(relayPin, power->getNewVal() >= 1 ? LOW : HIGH);
    digitalWrite(statusPin, power->getNewVal() >= 1 ? HIGH : LOW);

    Serial.printf("Relay remotely set to %s\n\r", power->getNewVal() >= 1 ? "ON" : "OFF");

    return true;
  }
};
