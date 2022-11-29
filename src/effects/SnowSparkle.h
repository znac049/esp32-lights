#ifndef _SNOW_SPARKLE_H_
#define _SNOW_SPARKLE_H_

class SnowSparkleEffect : public Effect {
    protected:
        CRGB backgroundColour = CRGB::Cyan;

    public:
        virtual const char *getName();
        virtual void reset(int _numLEDs, int _numStrings, int _loopDelay);
        virtual void loop();
};

#endif