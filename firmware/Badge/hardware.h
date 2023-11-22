#ifndef HARDWARE_H
#define HARDWARE_H

#ifndef BOARD_NAME
	#ifdef USB_VID
		#define BOARD_NAME "ESP32-S3"
		#define ESP32_S3
		// #define MININO
	#else
		#define BOARD_NAME "ESP32"
		#define ESP32_DEVKIT
	#endif
#endif

#if defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_RP2040)
#define RP2040
#endif

#if defined(ARDUINO_ARCH_RP2040)
#ifndef ARDUINO_ARCH_MBED_RP2040
#define ARCH_RP2040
#endif
#endif

#define DEBOUNCE_TIME_MS 70
#define LONG_CLICK_TIME_MS 500
#define BACK_ANIMATION_TIME_MS 150

#define BUTTON_LEFT 27   // GPIO27
// #define BUTTON_RIGHT 26  // GPIO26 -> For development in Raspberry Pi Pico
#define BUTTON_RIGHT 25  // GPIO25 -> For production in BugCON badge

#if defined(ESP32_DEVKIT)
#define TOUCH_LEFT 14    // GPIO14
#define TOUCH_RIGHT 12   // GPIO12
#elif defined(MININO)
#define TOUCH_LEFT 9    // GPIO9
#define TOUCH_RIGHT 10   // GPIO10
#else // ESP32_S3
#define TOUCH_LEFT 8    // GPIO8
#define TOUCH_RIGHT 7   // GPIO7
#endif
#define OLD_TOUCH_LEFT 18    // GPIO18

#if defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_RP2040)
// #define NEOPIXELS_PIN 16  // GPIO23 -> For development in Raspberry Pi Pico
#define NEOPIXELS_PIN 23  // For production in BugCON badge
#elif defined(ESP32_DEVKIT)
#define NEOPIXELS_PIN 19
#elif defined(MININO)
#define NEOPIXELS_PIN 11
#else // ESP32_S3
#define NEOPIXELS_PIN 38
#endif

#define NUMPIXELS 4

#endif