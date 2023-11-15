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
  void setTalkName(String talkName);
  String getID();
  void sendTalkName();
  void receiveTalkName();
  void enableCommunication();
  void disableCommunication();
  bool isCommunicationEnabled();
  std::vector<String> getTalkList();
};

#endif