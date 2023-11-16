#include "UartCommunication.h"

const char *validTalks[] = {
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Cloud Security -  The Blue Team Way",                                                                                                                              // 0
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Hacking Azure",                                                                                                                                                    // 1
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "DevSecOps",                                                                                                                                                        // 2
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "101 con Frida,    para hacking mobile",                                                                                                                            // 3
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "OSINT for         Offensive Security.  Top 10 herramientas  mas populares -      Evelyn Hernandez",                                                                // 4
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "100               vulnerabilidades en  30 dias: Hackeando   la mayor compania de seguros del mundo -  Alan Levy / Damian",                                         // 5
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Necesitas un      equipo de respuesta  ante ciberataques y  no sabes por donde   comenzar? - Marco    Mendoza / Gabriela",                                         // 6
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Tales From a Cloud CSIRT- Lets deep    dive into a          Kubernetes (k8s)     Infection            - Santiago Abastante",                                       // 7
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Como probar       nuestro plan de      respuesta a          incidentes de manera eficaz? - Julio César Muñoz Hernández",                                           // 8
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Brute Forcing and Destroy with IA      - Debug Sec",                                                                                                               // 9
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Evolving the      Threat Landscape:    The Rise of     Post-exploitation         - Maria Isabel       Manjarrez",                                                  // 10
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Querida,          descentralice a los  ninos                - St4t3",                                                                                              // 11
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Revelando el      talon de Aquiles:    explorando trucos de ingenieria social    usando aplicaciones  con interaccion      humana directa       - Mauro Parra",    // 12
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "El Lado Oscuro de la Concientizacion:  Como el Phishing     Interno Puede Salvar a tu Empresa - Jesus Barajas",                                                    // 13
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Rate limit es     importante? -  Felix Morales",                                                                                                                   // 14
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Hacking Azure -   Joaquin Molina",                                                                                                                                  // 15
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Como meterse a la fila de forma        elegante - Virgilio  Castro",                                                                                               // 16
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "NLPs y NERs:      Hablando y           compartiendo datos en modelos de Machine  Learning en un       lenguaje seguro -    Salvador Mendoza",                      // 17
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Bug bounty 101    - Adrian Pedrazzoli",                                                                                                                            // 18
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Security or       Espionage - Dhave    Security",                                                                                                                  // 19
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Advanced          Persistent Threat -  1: Como              desestabilizar un    pais - Jonathan      Palacios",                                                   // 20
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Definiendo        requisitos de        seguridad sin morir  en el intento        - Arturo Sustaita",                                                               // 21
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Mi camino a los   2500 puntos de       reputacion en        Hackerone            - Adrian Pedrazzoli",                                                             // 22
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "pulsioximetro IoT usando Arduino y     Node MCU             - Wulfrano Moreno",                                                                                    // 23
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "De BugCON a       Almoloya: full       disclosure           - Luis Velazquez",                                                                                     // 24
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Me fui por bug    bounty mobile y esto me paso              - Jacobo Tibaquira",                                                                                   // 25
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Porque todos      vulneran pero nadie  hace contencion y    mitigacion           - Irving Paz",                                                                    // 26
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Hacking etico     potenciado por       inteligencia         artificial: El       matrimonio perfecto  - Raul Aguilera      Andrade / Luis Daniel Aguirre Cueva",   // 27
    /*------col0------+--------col1--------+--------col2--------+--------col3--------+--------col4--------+--------col5--------+--------col6--------+--------col7--------+*/
    "Cómo hacer        ciberseguridad       Aplicando            Inteligencia         Artificial           - Pablo Corona"};                                            // 28

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
  debug.println("Setting talk: " + this->talkName);
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
    Serial2.println(this->talkName);
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
