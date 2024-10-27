# Thermostat

Digital thermostat based on ESP32 C3 SuperMini to control gas boiler via Home Kit.

Components used in the project:

- [ESP32 C3 dev board](https://arduino.ua/prod6618-mikroplata-esp32-c3-type-c-12-gpio)
- [Relay module 5V, 10A](https://arduino.ua/prod202-modyl-rele-5v-10a-s-optorazvyazkoi)
- ? [DC-DC step-down converter](https://arduino.ua/prod605-dc-dc-ponijaushhii-preobrazovatel-3a)
- [Power switch](https://arduino.ua/prod5433-vimikach-kcd1-106-2p-klavishnii-mini-jovtii)
- [USB Type-C socket with XH2.54-4P](https://arduino.ua/prod7084-plata-perehidnik-usb-type-c-f-usb2-0-to-xh2-54-4p)
- [XH2.54-4P Connector](https://arduino.ua/prod7005-kabel-z-klemnou-kolodkou-xh2-54-4p)
- [Universal box](https://arduino.ua/prod3207-korpys-plastikovii-dlya-elektroniki-d110a-komplekt-36x92x110mm)
- [Power supply](https://arduino.ua/prod6958-blok-pitaniya-usb-type-c-5v-5a)

In fact anything else can be used with the same nominals except ESP32 C3, this is just an example of components I have used.

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

The Relay module used supported 5V control signal, but ESP32 C3 board operates on 3.3V. Relay module was modified to support control signal on 3.3V (as [suggested](https://arduino.ua/prod202-modyl-rele-5v-10a-s-optorazvyazkoi) by `Kutsenko Oleksii (27.09.2023)`) by replacing R1, R2 to 1kOm instead of original 4.7kOm.

## Setup

- Copy `secrets.h.example` to `secrets.h`
- Configure WiFi credentials in `secrets.h`
- Figure out IP address of your board once connected (or assign IP to board MAC statically on your router which is recommended)

## Development

### USB Serial

Comment line with `monitor_port` and add next config to `platformio.ini` to enable on boot USB serial:

```ini
monitor_rts = 0
monitor_dtr = 0
build_flags = 
	-DCORE_DEBUG_LEVEL=5
	-DARDUINO_USB_MODE=1
	-DARDUINO_USB_CDC_ON_BOOT=1
```

### OTA

Firmware upgrade (OTA) enabled by default using PlatformIO, keep next configs with IP address of your board

```ini
upload_protocol = espota
upload_port = 192.168.0.10
```

If you messed up with OTA codebase and it is no longer working - comment lines above and flash firmware via USB.

