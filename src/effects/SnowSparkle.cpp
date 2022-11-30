#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "SnowSparkle.h"

const char *SnowSparkleEffect::getName(void) {
    return "Snow Sparkle";
}

void SnowSparkleEffect::reset(int _numLEDs, int _numStrings, int _loopDelay)
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);
    
    setAll(backgroundColour); 
}

void SnowSparkleEffect::loop()
{
    int led = random(numLEDs);

    setLED(led, CRGB::White);
    show();

    delay(loopDelay*2);
    setLED(led, backgroundColour);
    show();
  
    delay(loopDelay);
}

