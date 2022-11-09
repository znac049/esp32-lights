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
    insertIndex = removeIndex = 0;
}

void RandomTwinkleEffect::loop()
{
    clearAll();

    for (int i=0; i<Settings::density; i++) {
        int led = random(Settings::numLEDs);
        
        setLED(led, random(0,255), random(0,255), random(0,255));
        show();

        delay(Settings::loopDelay);
    }
}