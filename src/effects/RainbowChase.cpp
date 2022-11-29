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
 
    for (int q=0; q < 3; q++) {
        for (int i=0; i<Settings::getInt("numLEDs"); i+=3) {
            c = Wheel( (i+colourNum) % 255);
            setLED(i+q, *c++, *c++, *c++);
        }
        show();
       
        delay(Settings::getInt("loopDelay"));
       
        for (int i=0; i<Settings::getInt("numLEDs"); i+=3) {
            setLED(i+q, CRGB::Black);
        }
    }

    colourNum++;
    if (colourNum >= 256) {
        colourNum = 0;
    }
}