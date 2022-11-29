#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "StringChase.h"

int StringChaseEffect::sequence[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Cyan, CRGB::Magenta, CRGB::Yellow};

const char *StringChaseEffect::getName(void) {
    return "String Chase";
}

void StringChaseEffect::reset(int _numLEDs, int _numStrings, int _loopDelay)
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);    
}

void StringChaseEffect::loop()
{
    int numLEDs = Settings::getInt("numLEDs");
    int loopDelay = Settings::getInt("loopDelay") * 10;
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
