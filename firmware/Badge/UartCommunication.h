#ifndef UARTCOMMUNICATION_H
#define UARTCOMMUNICATION_H

#include <ArduinoUniqueID.h>

#include <vector>

#include "debug.h"
#include "hardware.h"

class UartCommunication {
 private:
  String ID;
  Debug debug;
  String talkName;
  bool communication;
  std::vector<String> talkList;
  void updateTalkList(String talkName);

 public:
  UartCommunication();
  bool receivedSuccess;
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