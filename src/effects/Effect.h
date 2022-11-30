#ifndef _EFFECT_H_
#define _EFFECT_H_

extern CRGB leds[MAX_LEDS];

class Effect {
    protected:
        byte *Wheel(byte WheelPos);
        int numLEDs;
        int numStrings;
        int stringLen;
        int loopDelay;
        
    public:
        virtual const char *getName();
        virtual void loop();
        virtual void reset(int _numLEDs, int _numStrings, int _loopDelay);
        virtual void changesMade();

        void clearAll();
        void setAll(CRGB colour);
        void setAll(int r, int g, int b);
        void setString(int stringNum, CRGB colour);
        void setLED(int lebNum, CRGB colour);
        void setLED(int stringNum, int lebNum, CRGB colour);
        void setLED(int ledNum, int r, int g, int b);
        void show();
        int percentOrValue(int pcnt, int val);
};

#endif