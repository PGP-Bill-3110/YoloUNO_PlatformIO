#ifndef ___MAIN_SERVER__
#define ___MAIN_SERVER__
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "global.h"

extern WebServer server;

//extern bool isAPMode;

String mainPage();
String settingsPage();

void startAP_main();
void setupServer();
void connectToWiFi_mainserver();

void main_server_task(void *pvParameters);

#endif