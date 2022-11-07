#include <FastLED.h>

#include "defs.h"
#include "SmartLED.h"
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
  for (int i=0; i<numLEDs; i++) {
    leds[i] = colours[i%15];
    smartLeds[i].step(i*5);
  }

  FastLED.show();
}

void JubileeEffect::loop()
{
  for (int i=0; i<numLEDs; i++) {
    smartLeds[i].step();
    leds[i] = smartLeds[i].getCurrent();
  }

  //Serial.print("-");
  FastLED.show();
  delay(20);
}