# Thermostat

Digital thermostat based on ESP32 C3 SuperMini to control gas boiler via Home Kit.

TODO. Currently this device acts as Home Kit switch. Add temperature sensor to implement real thermostat function.

Components used:

- [ESP32 C3 dev board](https://arduino.ua/prod6618-mikroplata-esp32-c3-type-c-12-gpio)
- [Relay module 5V, 10A](https://arduino.ua/prod202-modyl-rele-5v-10a-s-optorazvyazkoi)
- [Power switch](https://arduino.ua/prod5433-vimikach-kcd1-106-2p-klavishnii-mini-jovtii)
- [USB Type-C socket with XH2.54-4P](https://arduino.ua/prod7084-plata-perehidnik-usb-type-c-f-usb2-0-to-xh2-54-4p)
- [XH2.54-4P Connector](https://arduino.ua/prod7005-kabel-z-klemnou-kolodkou-xh2-54-4p)
- [Universal box](https://arduino.ua/prod3207-korpys-plastikovii-dlya-elektroniki-d110a-komplekt-36x92x110mm)
- [Power supply](https://arduino.ua/prod6958-blok-pitaniya-usb-type-c-5v-5a)

In fact anything else can be used with the same nominals and chip architecture, this is just an example of components I have used.

## Architecture

### Connection

- USB Type-C is used as power supply to get `input 5V`, `input GND`
- Power switch is connected to `input 5V`
- ESP32 board is connected to `input 5V` through Power switch via `5V` pin
- ESP32 board is connected to `input GND` via `G` pin
- Relay module is connected to `input 5V` through Power switch via `VCC` pin
- Relay module is connected to `input GND` via `Gnd` pin
- Relay module is connected to ESP32 board `GPIO5` pin via `In` pin
- Relay module out pads `NO` and `COM` should be connected to gas boiler thermostat's pads

### Operation

ESP32 board connects to home WiFi network and act as a smart switch in Home Kit environment. 

When switch is ON - ESP32 board sets logic 1 on Relay which shorts two output pads of the Relay. This gives the gas boiler a signal to enable heating as soon as thermostat pads shorted.

When switch is OFF - ESP32 board sets logic 0 on Relay which cur two output pads of the Relay. This tells the gas boiler a signal to stop heating.

TODO. Read temperature from other home devices to enable/disable heating automatically.
TODO. Connect temperature sensor to ESP32 board and expose Thermostat device to Home Kit to act automatically by keeping target temperature.

### Note For Relay Module

---The Relay module used supported 5V control signal, but ESP32 C3 board operates on 3.3V. Relay module was modified to support control signal on 3.3V (as [suggested](https://arduino.ua/prod202-modyl-rele-5v-10a-s-optorazvyazkoi) by `Kutsenko Oleksii (27.09.2023)`) by replacing R1, R2 to 1kOm instead of original 4.7kOm.---

Relay module declared to operate on 5V, and ESP32 C3 board declared to operate on 3.3V. However, it was discovered that ESP32 C3 board having powered by `5V` pin with 5 volts power operates normally with Relay module (that expects 5V on `In` pin) even with 3.3V as output on `GPIO5` which corresponds to logical 1. By measuring voltage at the time of logical 1 on `GPIO5` I can see voltage around 3.8V, which may be enough for Relay module to switch.

## Setup

- Copy `secrets.h.example` to `secrets.h`
- Configure WiFi credentials in `secrets.h`
- (optional) Assign IP to ESP32 C3 board MAC statically on your router

