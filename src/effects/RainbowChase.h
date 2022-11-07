#ifndef _RAINBOW_CHASE_H_
#define _RAINBOW_CHASE_H_

class RainbowChaseEffect : public Effect {
    private:
        int speedDelay = 20;
        
    public:
        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif