#ifndef AIRTAG_H
#define AIRTAG_H

#include "Arduino.h"
#include "hardware.h"

#if defined(ESP32)
#include <ArduinoBLE.h>
// #endif

// #if defined(ESP32)

// #include <ArduinoBLE.h>

class Airtag {
 private:
  BLEService myService;
  BLEIntCharacteristic myCharacteristic;

 public:
  Airtag();
  void loop();
  void start();
  void stop();
};

#endif
#endif  // AIRTAG_H