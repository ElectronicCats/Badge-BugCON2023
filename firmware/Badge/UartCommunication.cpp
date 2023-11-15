#include "UartCommunication.h"

const char *validTalks[] = {
    "Cloud Security - The Blue Team Way",                                                               // 0
    "Hacking Azure",                                                                                    // 1
    "DevSecOps",                                                                                        // 2
    "101 con Frida, para hacking mobile",                                                               // 3
    "OSINT for Offensive Security. Top 10 herramientas mas populares",                                  // 4
    "100 vulnerabilidades en 30 dias: Hackeando la mayor compania de seguros del mundo",                // 5
    "Necesitas un equipo de respuesta ante ciberataques y no sabes por donde comenzar?",                // 6
    "Tales From a Cloud CSIRT- Let’s deep dive into a Kubernetes (k8s) Infection",                      // 7
    "Como probar nuestro plan de respuesta a incidentes de manera eficaz?",                             // 8
    "Brute Forcing and Destroy with IA",                                                                // 9
    "Evolving the Threat Landscape: The Rise of Post-exploitation",                                     // 10
    "Querida, descentralice a los ninos",                                                               // 11
    "Mauro Parra",                                                                                      // 12
    "El Lado Oscuro de la Concientizacion: Como el Phishing Interno Puede Salvar a tu Empresa",         // 13
    "Rate limit es importante?",                                                                        // 14
    "Hacking Azure",                                                                                    // 15
    "Como meterse a la fila de forma elegante",                                                         // 16
    "NLPs y NERs: Hablando y compartiendo datos en modelos de Machine Learning en un lenguaje seguro",  // 17
    "Bug bounty 101",                                                                                   // 18
    "Security or Espionage",                                                                            // 19
    "Advanced Persistent Threat - 1: Como desestabilizar un pais",                                      // 20
    "Definiendo requisitos de seguridad sin morir en el intento",                                       // 21
    "Mi camino a los 2500 puntos de reputacion en Hackerone",                                           // 22
    "pulsioximetro IoT usando Arduino y Node MCU",                                                      // 23
    "Luis Velazquez",                                                                                   // 24
    "Me fui por bug bounty mobile y esto me paso",                                                      // 25
    "Porque todos vulneran pero nadie hace contencion y mitigacion",                                    // 26
    "Hacking etico potenciado por inteligencia artificial: El matrimonio perfecto",                     // 27
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
  this->receivedSuccess = false;
  debug.enable();
}

void UartCommunication::updateTalkList(String talkName) {
  talkName.trim();
  // Check if talkName is valid
  bool isValid = false;
  for (size_t i = 0; i < sizeof(validTalks) / sizeof(validTalks[0]); i++) {
    if (strcmp(validTalks[i], talkName.c_str()) == 0) {
      debug.println("'" + String(validTalks[i]) + "' == '" + talkName + "'");
      isValid = true;
      break;
    }
  }

  if (!isValid) {
    // debug.println("Invalid talk name: '" + talkName + "'");
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

  this->receivedSuccess = true;
}

void UartCommunication::setTalk(uint8_t index) {
  this->talkName = String(validTalks[index]);
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

    // debug.println("Received: " + input);

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

std::vector<String> UartCommunication::getTalkList() {
  return this->talkList;
}
