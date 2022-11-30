#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "StringChase.h"

int StringChaseEffect::sequence[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Cyan, CRGB::Magenta, CRGB::Yellow};
int StringChaseEffect::baseColour = 0;

const char *StringChaseEffect::getName(void) {
    return "String Chase";
}

void StringChaseEffect::reset(int _numLEDs, int _numStrings, int _loopDelay)
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);    
}

int StringChaseEffect::pickColour(int base)
{
    const int numColours = sizeof(sequence) / sizeof(int);
     
    base = base % numColours;
    return sequence[base];
}

void StringChaseEffect::loop()
{
    int nColours = sizeof(sequence) / sizeof(int);

    for (int i=0; i<numStrings; i++) {
        setString(i, pickColour(baseColour+i));
    }

    baseColour++;
    if (baseColour >= nColours) {
        baseColour = 0;
    }

    show();
    delay(loopDelay*25);
}
