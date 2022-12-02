#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "RainbowChase.h"

const char *RainbowChaseEffect::getName(void) {
    return "Rainbow Chase";
}

void RainbowChaseEffect::reset(int _numLEDs, int _numStrings, int _loopDelay)
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);    
}

void RainbowChaseEffect::loop()
{
    byte *c;
    const int step = 4;
 
    for (int q=0; q < 3; q++) {
        for (int i=0; i<numLEDs; i+=step) {
            c = Wheel( (i+colourNum) % 255);
            setLED(i+q, *c++, *c++, *c++);
        }
        show();
       
        delay(loopDelay);
       
        for (int i=0; i<numLEDs; i+=step) {
            setLED(i+q, CRGB::Black);
        }
    }

    colourNum++;
    if (colourNum >= 256) {
        colourNum = 0;
    }
}