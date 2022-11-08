#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
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
  for (int i=0; i<Settings::numLEDs; i++) {
    setLED(i, CRGB::Red);
  }
  show();
  delay(1000);

  for (int i=0; i<Settings::numLEDs; i++) {
    setLED(i, CRGB::Green);
  }
  show();
  delay(1000);

  for (int i=0; i<Settings::numLEDs; i++) {
    setLED(i, CRGB::Blue);
  }
  show();
  delay(1000);

  for (int i=0; i<Settings::numLEDs; i++) {
    setLED(i, CRGB::Black);
  }
  show();
  delay(1000);    
}