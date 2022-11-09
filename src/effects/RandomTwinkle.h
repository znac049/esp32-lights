#ifndef _RANDON_TWINKLE_H_
#define _RANDOM_TWINKLE_H_

class RandomTwinkleEffect : public Effect {
    private:
        int fifo[MAX_LEDS];
        int insertIndex;

        int twinkleCount;

    public:
        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif