# Badge BugCON 2023

## Setup

### Modify I2C pins

In order to use the SSD1306 display, you need to modify the I2C pins in `pins_arduino.h` to match the ones used in this board.

You can locate this file in the following path (depending on your OS):

- Windows: `C:\Users\deimos\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\4.0.6\variants\RASPBERRY_PI_PICO\pins_arduino.h`
- Linux: `~/.arduino15/packages/arduino/hardware/mbed_rp2040/4.0.6/variants/RASPBERRY_PI_PICO/pins_arduino.h`
- macOS: `/Users/deimos/Library/Arduino15/packages/arduino/hardware/mbed_rp2040/4.0.6/variants/RASPBERRY_PI_PICO/pins_arduino.h`

You can keep the original pins commented out, and add the following lines:

```cpp
// Wire
// #define PIN_WIRE_SDA        (4u)
// #define PIN_WIRE_SCL        (5u)
#define PIN_WIRE_SDA        (12u)
#define PIN_WIRE_SCL        (13u)
```
 
## Maintainer

<a
href="https://github.com/sponsors/ElectronicCats">

<img  src="https://electroniccats.com/wp-content/uploads/2020/07/Badge_GHS.png"  height="104" />

</a>

Electronic Cats invests time and resources providing this open source design, please support Electronic Cats and open-source hardware by purchasing products from Electronic Cats!