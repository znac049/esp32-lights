#include <WString.h>
#include <FastLED.h>

#include "defs.h"
#include "SmartLED.h"
#include "effects/Effect.h"
#include "effects/Jubilee.h"
#include "effects/Torpedo.h"
#include "effects/Simple.h"

#include "lights.h"

CRGB leds[MAX_LEDS];

JubileeEffect jub;
TorpedoEffect torp;
SimpleEffect simple;