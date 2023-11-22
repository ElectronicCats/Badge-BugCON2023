#include "Debug.h"

#ifdef ARDUINO_ARCH_MBED_RP2040
UART Serial2(8, 9, NC, NC);  // TX, RX, RTS, CTS
#endif

#ifdef ESP32_S3
#define TXD2 43
#define RXD2 44
#endif

Debug::Debug() {
  isEnable = false;
}

Debug& Debug::getInstance() {
  static Debug instance;
  return instance;
}

void Debug::begin(int baudRate) {
  Serial.begin(baudRate);
#ifdef RP2040
  Serial2.begin(baudRate);
#endif

#ifdef ESP32_S3
  Serial2.begin(baudRate, SERIAL_8N1, RXD2, TXD2);
#endif

  enable();
}

void Debug::enable() {
  isEnable = true;
}

void Debug::disable() {
  isEnable = false;
}

void Debug::waitForSerialConnection() {
	while (!Serial)
		;

#if defined(ESP32)
  for (int i = 0; i < 10; i++) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
#endif
}

void Debug::print(String message) {
  if (isEnable) {
    Serial.print(message);
  }
}

void Debug::println(String message) {
  if (isEnable) {
    Serial.println(message);
  }
}