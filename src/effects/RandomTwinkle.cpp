#include <FastLED.h>

#include "defs.h"
#include "Effect.h"
#include "RandomTwinkle.h"
#include "Settings.h"
#include "lights.h"

const char *RandomTwinkleEffect::getName(void) {
    return "Random Twinkle";
}

void RandomTwinkleEffect::reset()
{
    insertIndex = 0;
    twinkleCount = percentOrValue(Settings::density, Settings::numLEDs);
    for (int i=0; i<Settings::numLEDs; i++) {
        fifo[i] = Settings::numLEDs;
    }

    Serial.println("Twinkle count=" + String(twinkleCount));

    clearAll();
}

void RandomTwinkleEffect::loop()
{
    int led = random(Settings::numLEDs);

    setLED(fifo[insertIndex], CRGB::Black);
    fifo[insertIndex] = led;       
    setLED(led, random(0,255), random(0,255), random(0,255));
    show();

    insertIndex++;
    if (insertIndex >= Settings::numLEDs) {
        insertIndex = 0;
    }

    delay(Settings::loopDelay);
}