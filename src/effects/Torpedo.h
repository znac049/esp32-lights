#ifndef _TORPEDO_H_
#define _TORPEDO_H_

class TorpedoEffect : public Effect {
    public:
        virtual const char *getName();
        virtual void reset(int _numLEDs, int _numStrings, int _loopDelay);
        virtual void loop();
};

#endif