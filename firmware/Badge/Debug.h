#ifndef DEBUG_H
#define DEBUG_H

#include "Arduino.h"
#include "hardware.h"

class Debug {
 private:
  bool isEnable;
  Debug();

 public:
  static Debug& getInstance();
	void begin(int baudRate);
  void enable();
  void disable();
  void waitForSerialConnection();
  void print(String message);
  void println(String message);
};

#endif