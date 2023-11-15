#include "UartCommunication.h"

const char *validTalks[] = {
  "Nombre de la charla",
  "Cloud Security - The Blue Team Way",
  "Hacking Azure",
  "DevSecOps",
  "101 con Frida, para hacking mobile",
  "OSINT for Offensive Security. Top 10 herramientas más populares",
  "100 vulnerabilidades en 30 días: Hackeando la mayor compañía de seguros del mundo",
  "Necesitas un equipo de respuesta ante ciberataques y no sabes por dónde comenzar?",
  "Charla 8",
  "Charla 9",
  "Charla 10",
  "Charla 11",
  "Charla 12",
  "Charla 13",
  "Charla 14",
  "Charla 15",
  "Charla 16",
  "Charla 17",
  "Charla 18",
  "Charla 19",
  "Charla 20",
  "Charla 21",
  "Charla 22",
  "Charla 23",
  "Charla 24",
  "Charla 25",
  "Charla 26",
  "Charla 27",
  "Charla 28",
  "Charla 29",
  "Charla 30",
  "Charla 31",
  "Charla 32",
  "Charla 33",
  "Charla 34",
  "Charla 35",
};

UartCommunication::UartCommunication() {
  this->communication = false;
  debug.enable();
}

void UartCommunication::updateTalkList(String talkName) {
  talkName.trim();
  // Check if talkName is valid
  bool isValid = false;
  for (size_t i = 0; i < sizeof(validTalks) / sizeof(validTalks[0]); i++) {
    if(strcmp(validTalks[i], talkName.c_str()) == 0) {
      debug.println("'" + String(validTalks[i]) + "' == '" + talkName + "'");
      isValid = true;
      break;
    } else {
    }
  }

  if (!isValid) {
    debug.println("Invalid talk name");
    return;
  }

  debug.println("Updating talk list");

  // Check if talkName is already in the list
  for (size_t i = 0; i < talkList.size(); i++) {
    if (talkList[i] == talkName) {
      debug.println("Talk already in list");
      return;
    }
  }

  talkList.push_back(talkName);
  debug.println("Talk list size: " + String(talkList.size()));
  for (size_t i = 0; i < talkList.size(); i++) {
    debug.println("Talk " + String(i) + ": " + talkList[i]);
  }
}

void UartCommunication::setTalkName(String talkName) {
  this->talkName = talkName;
}

String UartCommunication::getID() {
  String ID = "";
  char buff[4];
  for (size_t i = 0; i < 8; i++) {
    sprintf(buff, "%02X", (uint8_t)UniqueID8[i]);
    ID += buff;
  }
  return ID;
}

void UartCommunication::sendTalkName() {
#if defined(ESP32)
  static unsigned long lastTime = millis();

  if (millis() - lastTime > 1000) {
    Serial.println(this->talkName);
    lastTime = millis();
  }
#endif
}

void UartCommunication::receiveTalkName() {
#ifdef RP2040
  if (Serial2.available()) {
    String input = Serial2.readStringUntil('\n');

    if (input.length() == 0) {
      debug.println("Empty input");
      return;
    }

    debug.println("Received: " + input);

    // TODO: validate with a list of valid talk names
    updateTalkList(input);
  }
#endif
}

void UartCommunication::enableCommunication() {
  this->communication = true;
}

void UartCommunication::disableCommunication() {
  this->communication = false;
}

bool UartCommunication::isCommunicationEnabled() {
  return this->communication;
}
