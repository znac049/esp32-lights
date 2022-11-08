#ifndef _EFFECT_H_
#define _EFFECT_H_

extern CRGB leds[MAX_LEDS];

class Effect {
    protected:
        int speedDelay = 20;
        int stepDelay = 20;

        byte *Wheel(byte WheelPos);
        
    public:
        //Effect();

        virtual const char *getName();
        virtual void loop();
        virtual void reset();
};

#endif