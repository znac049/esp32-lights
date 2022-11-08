#include <FastLED.h>

#include "defs.h"
#include "SmartLED.h"
#include "Settings.h"
#include "effects/Effect.h"
#include "effects/Jubilee.h"

const CRGB JubileeEffect::colours[] = {
    CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red, 
    CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::White, 
    CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue
};

const char *JubileeEffect::getName(void) {
    return "Jubilee";
}

void JubileeEffect::reset()
{
  for (int i=0; i<Settings::numLEDs; i++) {
    setLED(i, colours[i%15]);
    smartLeds[i].step(i*5);
  }

  show();
}

void JubileeEffect::loop()
{
  for (int i=0; i<Settings::numLEDs; i++) {
    smartLeds[i].step();
    setLED(i, smartLeds[i].getCurrent());
  }

  show();
  delay(stepDelay);
}