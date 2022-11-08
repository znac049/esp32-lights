#ifndef _RANDON_TWINKLE_H_
#define _RANDOM_TWINKLE_H_

class RandomTwinkleEffect : public Effect {
    private:
        int count = 12;
        bool onlyOne = false;

    public:
        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif