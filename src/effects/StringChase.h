#ifndef _STRING_CHASE_H_
#define _STRING_CHASE_H_

class StringChaseEffect : public Effect {
    private:
        static int sequence[];
        static int baseColour;

        int pickColour(int base);
        
    public:
        virtual const char *getName();
        virtual void reset(int _numLEDs, int _numStrings, int _loopDelay);
        virtual void loop();
};

#endif