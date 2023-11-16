# Badge BugCON 2023

Link: https://github.com/ElectronicCats/Badge-BugCON2023

Author(s): Francisco Torres

Last updated: 2023/11/09

## Developer guide

## Setup

### Libraries required

Install the following libraries:

- [Adafruit GFX Library](https://www.arduino.cc/reference/en/libraries/adafruit-gfx-library/)
- [Adafruit BusIO](https://www.arduino.cc/reference/en/libraries/adafruit-busio/)
- [Adafruit NeoPixel](https://www.arduino.cc/reference/en/libraries/adafruit-neopixel/)
- [Adafruit SSD1306](https://www.arduino.cc/reference/en/libraries/adafruit-ssd1306/)
- [ezButton](https://www.arduino.cc/reference/en/libraries/ezbutton/)
- [ArduinoUniqueID](https://www.arduino.cc/reference/en/libraries/arduinouniqueid/)
- [Adafruit SH110X](https://www.arduino.cc/reference/en/libraries/adafruit-sh110x/)

> You can install these libraries using the Arduino IDE Library Manager (Sketch > Include Library > Manage Libraries...).

Versions used in this project:

```bash
Used library         Version    
Adafruit GFX Library 1.11.7
Adafruit BusIO       1.14.3
Wire
SPI
Adafruit NeoPixel    1.11.0
Adafruit SH110X      2.1.8
Adafruit SSD1306     2.5.7
ezButton             1.0.4
ArduinoUniqueID      1.3.0
```

> You can use latest versions of these libraries.

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

### Fix SSD1306 error

Try to compile the project, if you get the following error:

```bash
❯ make compile BOARD=rp2040
arduino-cli compile --fqbn arduino:mbed_rp2040:pico --export-binaries
/Users/deimos/Documents/Arduino/libraries/Adafruit_SSD1306/Adafruit_SSD1306.cpp:42:10: fatal error: pgmspace.h: No such file or directory
 #include <pgmspace.h>
          ^~~~~~~~~~~~
compilation terminated.
```

You need to fix the following error by commenting out the following line in `Adafruit_SSD1306.cpp`:

```cpp
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
// #include <pgmspace.h>
#else
```

> More about this error: https://github.com/adafruit/Adafruit_SSD1306/issues/259

### Production

In `hardware.h` change the following lines:

```cpp
// #define BUTTON_RIGHT 26  // GPIO26 -> For development in Raspberry Pi Pico
#define BUTTON_RIGHT 25  // GPIO25 -> For production in BugCON badge

// #define NEOPIXELS_PIN 16  // GPIO23 -> For development in Raspberry Pi Pico
#define NEOPIXELS_PIN 23  // For production in BugCON badge
```

Generate binaries for each board:

```bash
make compile BOARD=rp2040
```

```bash
make compile BOARD=esp32s3
```

Compile and flash the binaries to the boards:

```bash
make all BOARD=rp2040
```

```bash
make all BOARD=esp32s3
```

> Note: Make sure you have the correct port selected in `Makefile` for each board.
 
## Maintainer

<a
href="https://github.com/sponsors/ElectronicCats">

<img  src="https://electroniccats.com/wp-content/uploads/2020/07/Badge_GHS.png"  height="104" />

</a>

Electronic Cats invests time and resources providing this open source design, please support Electronic Cats and open-source hardware by purchasing products from Electronic Cats!