#include <FastLED.h>

#include "defs.h"
#include "Effect.h"
#include "Simple.h"

const char *SimpleEffect::getName(void) {
    return "Simple";
}

void SimpleEffect::reset()
{

}

void SimpleEffect::loop()
{
  for (int i=0; i<numLEDs; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
  delay(1000);

  for (int i=0; i<numLEDs; i++) {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
  delay(1000);

  for (int i=0; i<numLEDs; i++) {
    leds[i] = CRGB::Blue;
  }
  FastLED.show();
  delay(1000);

  for (int i=0; i<numLEDs; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(1000);    
}