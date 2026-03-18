#ifndef HTTPGEN
#define HTTPGEN

#include <arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "InterfazWeb.h"

extern const char* ssid;
extern const char* password;
extern ESP8266WebServer server;

void handleRoot();

void HTTP_init();

String WiFiHTTP_handleReq();

#endif