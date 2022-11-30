#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "Simple.h"

int SimpleEffect::sequence[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Black};

const char *SimpleEffect::getName(void) {
    return "Simple";
}

void SimpleEffect::reset(int _numLEDs, int _numStrings, int _loopDelay)
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);    
}

void SimpleEffect::loop()
{
    int nColours = sizeof(sequence) / sizeof(int);

    for (int col=0; col<nColours; col++) {
        int colour = sequence[col];

        for (int i=0; i<numLEDs; i++) {
            setLED(i, colour);
        }
        show();
        delay(loopDelay);
    }
}
