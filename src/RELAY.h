#include <HomeSpan.h>

struct RELAY : Service::Switch {

  int relayPin;
  SpanCharacteristic *power;

  RELAY(int relayPin) : Service::Switch() 
  {
    power = new Characteristic::On();
    this->relayPin = relayPin;
    pinMode(relayPin, OUTPUT);
    digitalWrite(this->relayPin, HIGH); // turn off relay's Normally Open pad
  }

  boolean update() 
  {
    digitalWrite(relayPin, power->getNewVal() >= 1 ? LOW : HIGH);
    return true;
  }
};
