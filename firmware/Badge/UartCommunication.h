#ifndef UARTCOMMUNICATION_H
#define UARTCOMMUNICATION_H

#include <ArduinoUniqueID.h>

#include <vector>

#include "debug.h"
#include "hardware.h"
#ifdef RP2040
#include "FlashIAPBlockDevice.h"
#include "KVStore.h"
#include "TDBStore.h"
#endif

class UartCommunication {
 private:
  String ID;
  Debug& debug = Debug::getInstance();
  String talkName;
  bool communication;
  std::vector<String> talkList;
  uint8_t talksCounter;
  uint8_t talksCounterBuffer[1];
  uint8_t receivedTalksOrder[40];
  uint8_t receivedTalksOrderBuffer[40];
  void updateTalkList(String talkName);

 public:
  UartCommunication();
  bool receivedSuccess;
  void begin();
  void ereaseFlash();
  void setTalk(uint8_t index);
  String getID();
  void sendTalkName();
  void receiveTalkName();
  void enableCommunication();
  void disableCommunication();
  bool isCommunicationEnabled();
  std::vector<String> getTalkList();
};

#endif