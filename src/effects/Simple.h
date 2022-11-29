#ifndef _SIMPLE_H_
#define _SIMPLE_H_

class SimpleEffect : public Effect {
    private:
        static int sequence[];
        
    public:
        virtual const char *getName();
        virtual void reset(int _numLEDs, int _numStrings, int _loopDelay);
        virtual void loop();
};

#endif