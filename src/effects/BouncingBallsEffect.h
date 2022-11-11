#ifndef _BOUNCING_BALLS_H_
#define _BOUNCING_BALLS_H_

class BouncingBallsEffect : public Effect {
    public:
        const static int MAX_BALLS = 6;

    private:
        int ballCount = 6;
        float Gravity = -9.81;
        int StartHeight = 1;
        
        float ImpactVelocityStart = sqrt(-2 * Gravity * StartHeight);
        float Height[MAX_BALLS];
        float ImpactVelocity[MAX_BALLS];
        float TimeSinceLastBounce[MAX_BALLS];
        int   Position[MAX_BALLS];
        long  ClockTimeSinceLastBounce[MAX_BALLS];
        float Dampening[MAX_BALLS];
        CRGB colours[MAX_BALLS];
    
    public:
        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif