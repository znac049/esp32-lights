#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"
#include "BouncingBallsEffect.h"

const char *BouncingBallsEffect::getName(void) {
    return "Bouncing Balls";
}

void BouncingBallsEffect::reset(int _numLEDs, int _numStrings, int _loopDelay) 
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);
    
    for (int i = 0 ; i < ballCount ; i++) {  
        ClockTimeSinceLastBounce[i] = millis();
        Height[i] = StartHeight;
        Position[i] = 0;
        ImpactVelocity[i] = ImpactVelocityStart;
        TimeSinceLastBounce[i] = 0;
        Dampening[i] = 0.90 - float(i)/pow(ballCount,2);
    }
}

void BouncingBallsEffect::loop()
{   
    for (int i = 0 ; i < ballCount ; i++) {
        TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
        Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
    
        if ( Height[i] < 0 ) {                      
            Height[i] = 0;
            ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
            ClockTimeSinceLastBounce[i] = millis();
        
            if ( ImpactVelocity[i] < 0.01 ) {
                ImpactVelocity[i] = ImpactVelocityStart;
            }
        }

        Position[i] = round( Height[i] * (Settings::getInt("numLEDs") - 1) / StartHeight);

        for (i=0; i<MAX_BALLS; i++) {
            colours[i] = CRGB(random(255), random(255), random(255));
        }
    }
    
    for (int i = 0 ; i < ballCount ; i++) {
        setLED(Position[i], colours[i]);
    }
    
    show();
    clearAll();
}


void BouncingColoredBalls(int BallCount, byte colors[][3]) {
 }