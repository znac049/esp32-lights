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
}

void RandomTwinkleEffect::loop()
{
    for (int i=0; i<Settings::numLEDs; i++) {
        leds[i] = CRGB::Black;
    }

    for (int i=0; i<count; i++) {
        int led = random(Settings::numLEDs);

        leds[led] = CRGB(random(0,255), random(0,255), random(0,255));
        FastLED.show();

        delay(speedDelay);
        if (onlyOne) {
            //leds[led] = CRGB::Black;
            FastLED.show();
        }
    }
 
    delay(speedDelay);
}