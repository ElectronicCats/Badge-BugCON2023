# Badge BugCON 2023

Link: https://github.com/ElectronicCats/Badge-BugCON2023

Author(s): Francisco Torres

Last updated: 2023/01/03

## Developer guide

## Setup

### Modify I2C pins

In order to use the SSD1306 display, you need to modify the I2C pins in `pins_arduino.h` to match the ones used in these boards.

You can locate this file in the following path (depending on your OS):

#### For RP2040

- Windows: `C:\Users\user\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\4.0.6\variants\RASPBERRY_PI_PICO\pins_arduino.h`
- Linux: `~/.arduino15/packages/arduino/hardware/mbed_rp2040/4.0.6/variants/RASPBERRY_PI_PICO/pins_arduino.h`
- macOS: `~/Library/Arduino15/packages/arduino/hardware/mbed_rp2040/4.0.6/variants/RASPBERRY_PI_PICO/pins_arduino.h`

You can keep the original pins commented out, and add the following lines:

```cpp
// Wire
// #define PIN_WIRE_SDA        (4u)
// #define PIN_WIRE_SCL        (5u)
#define PIN_WIRE_SDA        (12u)
#define PIN_WIRE_SCL        (13u)
```

#### For ESP32-S3

- Windows: `C:\Users\user\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\variants\esp32s3\pins_arduino.h`
- Linux: `~/.arduino15/packages/esp32/hardware/esp32/2.0.11/variants/esp32s3/pins_arduino.h`
- macOS: `~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/variants/esp32s3/pins_arduino.h`

You can keep the original pins commented out, and add the following lines:

```cpp
// static const uint8_t SDA = 8;
// static const uint8_t SCL = 9;
static const uint8_t SDA = 3;
static const uint8_t SCL = 4;
```
 
## Maintainer

<a
href="https://github.com/sponsors/ElectronicCats">

<img  src="https://electroniccats.com/wp-content/uploads/2020/07/Badge_GHS.png"  height="104" />

</a>

Electronic Cats invests time and resources providing this open source design, please support Electronic Cats and open-source hardware by purchasing products from Electronic Cats!