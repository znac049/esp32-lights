#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <FastLED.h>

#include "defs.h"
#include "lightshow.h"
#include "effects/Effect.h"

// globals.cpp
extern CRGB leds[MAX_LEDS];
extern LightShow lightShow;

// leds.cpp
bool setupLEDs();

// main.cpp
extern int effectNum;
extern int numEffects;
extern bool dirty;
//extern bool blackout;
extern Effect *effects[];

// web/macro.cpp
extern String lookupMacro(const String& macroName);

// web/webserver.cpp
extern void setupWebserver();
extern void cleanupWebsocketClients();

// wifi.cpp
extern bool setupWifi();

#endif