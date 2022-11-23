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

void setup()
{
    Serial.begin(115200);
    Settings::loadRequired();

    Serial.println("Device name: " + Settings::get("deviceName"));
    blackout = false;

    setupLEDs(); 
    setupWifi();
    setupWebserver();

    // Jumper to determine if a nvram reset is needed
    pinMode(RESET_NVRAM_PIN, INPUT_PULLUP);
    delay(10);
    Settings::loadRequired();
    
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
inline int fadeDown(int gun)
{
    gun = gun -8;

    if (gun < 0)
        return 0;

    return gun;
}

void fade()
{
    // Fade down the LEDs
    for (int i=0; i<Settings::getInt("numleds"); i++) {
        if (leds[i].red > 0) { 
            leds[i].red = fadeDown(leds[i].red);
        }

        if (leds[i].green > 0) {
            leds[i].green = fadeDown(leds[i].green);
        }

        if (leds[i].blue > 0) {
            leds[i].blue = fadeDown(leds[i].blue);
        }
    }

    FastLED.show();
}

void loop()
{
    cleanupWebsocketClients();

    if (blackout) {
        fade();
    }
    else {
        if (dirty) {
            Serial.println("In main loop - changes to settings detected.");

            // Has the selected effect changed?
            if (runningEffect != effectNum) {
                effects[effectNum]->reset();
                runningEffect = effectNum;
            }

            FastLED.setBrightness(Settings::getInt("brightness"));

            // Give the effect a chance to change based on the changes made
            effects[effectNum]->changesMade();
            dirty = false;
        }

        effects[effectNum]->loop();
    }
}
