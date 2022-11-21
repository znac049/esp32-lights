#ifndef _SIMPLE_H_
#define _SIMPLE_H_

class SimpleEffect : public Effect {
    private:
        static int sequence[];
        
    public:
        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif