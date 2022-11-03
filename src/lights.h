#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#define MAX_LEDS 150

// main.cpp
extern bool ledState;
extern const int ledPin;

// webserver.cpp
extern void setupWebserver();
extern void cleanupWebsocketClients();

#endif