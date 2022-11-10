#ifndef _SNOW_SPARKLE_H_
#define _SNOW_SPARKLE_H_

class SnowSparkleEffect : public Effect {
    protected:
        CRGB backgroundColour = CRGB::Cyan;

    public:
        virtual const char *getName();
        virtual void reset();
        virtual void loop();
};

#endif