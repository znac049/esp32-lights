#include <WString.h>
#include <FastLED.h>

#include "defs.h"
#include "SmartLED.h"
#include "effects/effects.h"

#include "lights.h"

CRGB leds[MAX_LEDS];

JubileeEffect jub;
SimpleEffect simple;
RandomTwinkleEffect rte;
RainbowChaseEffect rainbow;
SnowSparkleEffect snow;
BouncingBallsEffect balls;
StringChaseEffect schase;

LightShow lightShow;
