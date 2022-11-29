#include <FastLED.h>

#include "defs.h"
#include "Effect.h"
#include "Torpedo.h"

const char *TorpedoEffect::getName(void) {
    return "Torpedo";
}

void TorpedoEffect::reset(int _numLEDs, int _numStrings, int _loopDelay)
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);
}

void TorpedoEffect::loop()
{   
}