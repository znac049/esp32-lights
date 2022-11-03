#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "lights.h"
#include "FastLED.h"

extern CRGB leds[MAX_LEDS];

class Effect {
    public:
        int numLEDs = 16;

    public:
        //Effect();

        virtual const char *getName();
        virtual void loop();
        virtual void reset();
};

#endif