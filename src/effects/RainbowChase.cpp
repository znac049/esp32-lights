#include <FastLED.h>

#include "defs.h"
#include "Effect.h"
#include "RainbowChase.h"

const char *RainbowChaseEffect::getName(void) {
    return "Rainbow Chase";
}

void RainbowChaseEffect::reset()
{
}

void RainbowChaseEffect::loop()
{
    byte *c;
 
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (int i=0; i<numLEDs; i+=3) {
                c = Wheel( (i+j) % 255);
                leds[i+q] = CRGB(*c, *(c+1), *(c+2));
            }
            FastLED.show();
       
            delay(speedDelay);
       
            for (int i=0; i<numLEDs; i+=3) {
                leds[i+q] = CRGB::Black;
            }
        }
    }
}