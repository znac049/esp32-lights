#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <FastLED.h>
#include <SPIFFS.h>

#include "defs.h"
#include "SmartLED.h"
#include "effects/effects.h"
#include "Settings.h"
#include "lightshow.h"

#include "lights.h"

extern JubileeEffect jub;
extern SimpleEffect simple;
extern RandomTwinkleEffect rte;
extern RainbowChaseEffect rainbow;
extern SnowSparkleEffect snow;
extern BouncingBallsEffect balls;
extern StringChaseEffect schase;

int runningEffect=0;
//int effectNum=0;

Effect *effects[] = {
    dynamic_cast<Effect*>(&rte),
    dynamic_cast<Effect*>(&jub),
    dynamic_cast<Effect*>(&simple),
    dynamic_cast<Effect*>(&rainbow), 
    dynamic_cast<Effect*>(&snow), 
    dynamic_cast<Effect*>(&balls), 
    dynamic_cast<Effect*>(&schase), 
};

int numEffects = (sizeof(effects) / sizeof(Effect *));

void setup()
{
    Serial.begin(115200);
    Settings::begin();

    Serial.println("Device name: " + Settings::get("deviceName"));

    if (!SPIFFS.begin(true)) {
        Serial.println("Error mounting /spiffs filesystem.");
    }

    setupLEDs(); 
    setupWifi();
    setupWebserver();

    lightShow.addEffect(&rte);
    lightShow.addEffect(&jub);
    lightShow.addEffect(&simple);
    lightShow.addEffect(&rainbow);
    lightShow.addEffect(&snow);
    lightShow.addEffect(&balls);
    lightShow.addEffect(&schase);
    lightShow.setup();

    // Jumper to determine if a nvram reset is needed
    pinMode(RESET_NVRAM_PIN, INPUT_PULLUP);
    delay(2);
}

void loop()
{
    lightShow.run();

    Serial.printf("ERROR: lightShow.run() returned. This should never happen!\n");
}
