#ifndef _TORPEDO_H_
#define _TORPEDO_H_

class TorpedoEffect : public Effect {
    public:
        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif