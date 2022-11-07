#ifndef _EFFECT_H_
#define _EFFECT_H_

extern CRGB leds[MAX_LEDS];

class Effect {
    public:
        int numLEDs = 16;

    protected:
        byte *Wheel(byte WheelPos);
        
    public:
        //Effect();

        virtual const char *getName();
        virtual void loop();
        virtual void reset();
};

#endif