#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <FastLED.h>

#include "defs.h"
#include "SmartLED.h"
#include "effects/effects.h"
#include "Settings.h"

#include "lights.h"

extern JubileeEffect jub;
extern TorpedoEffect torp;
extern SimpleEffect simple;
extern RandomTwinkleEffect rte;
extern RainbowChaseEffect rainbow;
extern SnowSparkleEffect snow;
extern BouncingBallsEffect balls;

int runningEffect=0;
int effectNum=0;

Effect *effects[] = {
    dynamic_cast<Effect*>(&rte),
    dynamic_cast<Effect*>(&jub),
    dynamic_cast<Effect*>(&torp),
    dynamic_cast<Effect*>(&simple),
    dynamic_cast<Effect*>(&rainbow), 
    dynamic_cast<Effect*>(&snow), 
    dynamic_cast<Effect*>(&balls), 
};

int numEffects = (sizeof(effects) / sizeof(Effect *));

//bool ledState = 0;
//const int ledPin = 21;
//String hostname;

void setup()
{
    Serial.begin(115200);
    Settings::loadRequired();

    Serial.println("Device name: " + Settings::get("deviceName"));
    blackout = false;

    setupLEDs(); 

    setupWifi();
    setupWebserver();
    
    effectNum = Settings::getInt("patternNumber");

    Serial.print("Pattern Number=");
    Serial.println(effectNum);

    if (effectNum >= numEffects) {
        effectNum = 0;
        Settings::set("patternNumber", effectNum);
    }

    dirty = false;
    runningEffect = effectNum;
    effects[effectNum]->reset();
}

void loop() {
    cleanupWebsocketClients();

    if (!blackout) {
        if (dirty) {
            Serial.println("In main loop - changes to settings detected.");

            // Has the selected effect changed?
            if (runningEffect != effectNum) {
                effects[effectNum]->reset();
                runningEffect = effectNum;
            }

            FastLED.setBrightness(Settings::getInt("brightness"));

            effects[effectNum]->changesMade();
            dirty = false;
        }
        effects[effectNum]->loop();
    }
    else {
        bool fading = false;

        // Fade down the LEDs
        for (int i=0; i<MAX_LEDS; i++) {
            if (leds[i].red > 0) { 
                leds[i].red--;
                fading = true;
            }

            if (leds[i].green > 0) {
                leds[i].green--;
                fading = true;
            }

            if (leds[i].blue > 0) {
                leds[i].blue--;
                fading = true;
            }
        }

        if (fading) {
            Serial.println("Fading down to blackout");
        }
    }
}
