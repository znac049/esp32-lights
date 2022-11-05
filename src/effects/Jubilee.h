#ifndef _JUBILEE_H_
#define _JUBILEE_H_

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