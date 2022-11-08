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
    speedDelay = 40;
}

void RandomTwinkleEffect::loop()
{
    clearAll();

    for (int i=0; i<count; i++) {
        int led = random(Settings::numLEDs);
        
        setLED(led, random(0,255), random(0,255), random(0,255));
        show();

        delay(speedDelay);
        if (onlyOne) {
            setLED(led, CRGB::Black);
            show();
        }
    }
 
    delay(speedDelay);
}