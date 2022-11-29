#ifndef _RANDON_TWINKLE_H_
#define _RANDOM_TWINKLE_H_

class RandomTwinkleEffect : public Effect {
    private:
        int history[MAX_LEDS];
        int insert;
        int remove;

        int twinkleCount;
        int numLEDs;
        int loopDelay;

        void lightLed(int ledNum, int r, int g, int b);
        void lightLed(int ledNum, CRGB col);
        CRGB pickColour();

    public:
        virtual const char *getName();
        virtual void reset(int _numLEDs, int _numStrings, int _loopDelay);
        virtual void loop();
        virtual void changesMade();
};

#endif