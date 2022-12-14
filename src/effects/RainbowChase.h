#ifndef _RAINBOW_CHASE_H_
#define _RAINBOW_CHASE_H_

class RainbowChaseEffect : public Effect {
    private:
        int colourNum=0;

    public:
        virtual const char *getName();
        virtual void reset(int _numLEDs, int _numStrings, int _loopDelay);
        virtual void loop();
};

#endif