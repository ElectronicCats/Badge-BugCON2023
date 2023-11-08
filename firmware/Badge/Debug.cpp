#include "Debug.h"

#ifdef RP2040
UART Serial2(8, 9, NC, NC);  // TX, RX, RTS, CTS
#endif

Debug::Debug() {
  isEnable = false;
}

void Debug::begin(int baudRate) {
  Serial.begin(baudRate);
#ifdef RP2040
  Serial2.begin(baudRate);
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