#ifndef _EFFECT_H_
#define _EFFECT_H_

extern CRGB leds[MAX_LEDS];

class Effect {
    protected:
        byte *Wheel(byte WheelPos);
        
    public:
        //Effect();

        virtual const char *getName();
        virtual void loop();
        virtual void reset();

        void clearAll();
        void setAll(CRGB colour);
        void setAll(int r, int g, int b);
        void setLED(int lebNum, CRGB colour);
        void setLED(int ledNum, int r, int g, int b);
        void show();
        int percentOrValue(int pcnt, int val);
};

#endif