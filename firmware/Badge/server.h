#ifndef SERVER_H
#define SERVER_H

#include "hardware.h"

#ifdef ESP32

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

#include "debug.h"

enum URL {
  INDEX_HTML = 0,
  MAIN_JS,
  STYLES_CSS,
  NOT_FOUND
};

class WebServer {
 private:
  int port = 80;
  WiFiServer server;
  String defaultSSDI;
  String defaultPassword;
  int status;
  URL webRequest;
  Debug &debug = Debug::getInstance();
  void loadPageContent(WiFiClient client);
  void showPageContent(WiFiClient client, const char *pageContent);
  String decodeURL(char *url);
  void updateWebRequest(String url);
  void handleRequests();
  void handleURLParameters(String url);

 public:
  WebServer();
  void begin();
  void loop();
};

#endif  // ESP32
#endif  // SERVER_H