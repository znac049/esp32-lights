#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "Simple.h"

int SimpleEffect::sequence[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Black};

const char *SimpleEffect::getName(void) {
    return "Simple";
}

void SimpleEffect::reset()
{
}

void SimpleEffect::loop()
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
