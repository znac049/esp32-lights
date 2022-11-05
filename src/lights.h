#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "defs.h"

struct Effects {
    String name;
    Effect& effect;
};

// globals.cpp
extern CRGB leds[MAX_LEDS];

// main.cpp
extern bool ledState;
extern const int ledPin;

// webserver.cpp
extern void setupWebserver();
extern void cleanupWebsocketClients();

#endif