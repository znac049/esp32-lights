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

int LightShow::numLEDs = 0;
int LightShow::numStrings = 0;
int LightShow::loopDelay = 0;

Effect *LightShow::effects[MAX_EFFECTS];

LightState LightShow::state = ST_IDLE;

LightShow::LightShow()
{
}

void LightShow::setup()
{
    settingsChanged();
}

void LightShow::changeState(LightState targetState)
{
    switch (state) {
        default:
            Serial.printf("Unknown state in LightShow state machine.\n");
            break;

        case ST_IDLE:
            Serial.printf("Current state is 'IDLE'\n");

            switch (targetState) {
                case ST_RUNNING:
                    state = targetState;
                    effects[currentEffect]->reset(numLEDs, numStrings, loopDelay);
                    break;
            }
            break;

        case ST_RUNNING:
            Serial.printf("Current state is 'RUNNING'\n");
            break;
    }
}

void LightShow::fade()
{
}

void LightShow::loop()
{
    cleanupWebsocketClients();
    effects[currentEffect]->loop();
}

void LightShow::settingsChanged()
{
    bool parallel = (Settings::getInt("parallel") == 0);
    int order = Settings::getInt("LEDOrder");
    int stringLen;

    numLEDs = Settings::getInt("numLEDs");
    numStrings = Settings::getInt("numStrings");
    stringLen = numLEDs / numStrings;    

    if (parallel) {
        numLEDs = stringLen;

        Serial.printf("Setting string length to %d and numLEDs to %d (for parallel use)\n", stringLen, numLEDs);
    }
    else {
        Serial.printf("Setting each string length to %d and numLEDs to %d (for sequential use)\n", stringLen, numLEDs);
    }

    reset();
}

void LightShow::reset()
{
    int effectNum = Settings::getInt("patternNumber");

    loopDelay = Settings::getInt("loopDelay");

    if ((effectNum >= 0) && (effectNum < numEffects)) {
        currentEffect = effectNum;
    }

    effects[currentEffect]->reset(numLEDs, numStrings, loopDelay);        
}

void LightShow::run()
{
    int effectNum = Settings::getInt("patternNumber");

    if ((effectNum >= 0) && (effectNum < numEffects)) {
        currentEffect = effectNum;
    }

    changeState(ST_RUNNING);

    Serial.printf("Preparing to run effect #%d: '%s'\n", currentEffect, effects[currentEffect]->getName());

    while (true) {
        // What's happened to make us drop out of loop()?
        switch (state) {
            default:
            case ST_RUNNING:
                cleanupWebsocketClients();
                loop();
                break;
        }
    }
}

void LightShow::addEffect(Effect *effect)
{
    Serial.printf("Registering new effect #%d: '%s'\n", numEffects, effect->getName());

    effects[numEffects] = effect;
    numEffects++;
}