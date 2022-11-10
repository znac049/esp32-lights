#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "SnowSparkle.h"

const char *SnowSparkleEffect::getName(void) {
    return "Snow Sparkle";
}

void SnowSparkleEffect::reset()
{
    setAll(backgroundColour); 
}

void SnowSparkleEffect::loop()
{
    int led = random(Settings::numLEDs);

    setLED(led, CRGB::White);
    show();

    delay(Settings::loopDelay);
    setLED(led, backgroundColour);
    show();
  
    delay(Settings::loopDelay);
}

