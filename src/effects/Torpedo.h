#ifndef _TORPEDO_H_
#define _TORPEDO_H_

class TorpedoEffect : public Effect {
    public:
        //TorpedoEffect() : Effect() {};

        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif