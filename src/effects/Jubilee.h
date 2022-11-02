#ifndef _JUBILEE_H_
#define _JUBILEE_H_

#include "lights.h"
#include "FastLED.h"
#include "SmartLED.h"
#include "Effect.h"

class JubileeEffect : public Effect {
    public:
        static const CRGB colours[];

    protected:
        SmartLED smartLeds[MAX_LEDS];

    public:
        //JubileeEffect() : Effect() {};

        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif