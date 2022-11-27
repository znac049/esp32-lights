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

#include "lights.h"

#include "lightshow.h"

int LightShow::numEffects = 0;
int LightShow::currentEffect = 0;

Effect *LightShow::effects[MAX_EFFECTS];

LightShow::LightShow()
{}

void LightShow::run()
{
    int effectNum = Settings::getInt("effectnum");

    if ((effectNum >= 0) && (effectNum < numEffects)) {
        currentEffect = effectNum;
    }

    Serial.printf("Preparing to run effect #%d: '%s'\n", currentEffect, effects[currentEffect]->getName());
}

void LightShow::addEffect(Effect *effect)
{
    Serial.printf("Registering new effect #%d: '%s'\n", numEffects, effect->getName());

    effects[numEffects] = effect;
    numEffects++;
}