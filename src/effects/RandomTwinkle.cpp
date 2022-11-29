#include <FastLED.h>

#include "defs.h"
#include "Effect.h"
#include "RandomTwinkle.h"
#include "Settings.h"
#include "lights.h"

const char *RandomTwinkleEffect::getName(void)
{
    return "Random Twinkle";
}

void RandomTwinkleEffect::reset(int _numLEDs, int _numStrings, int _loopDelay)
{
    Effect::reset(_numLEDs, _numStrings, _loopDelay);

    changesMade();
    clearAll();

    for (int i=0; i<MAX_LEDS; i++) {
        history[i] = -1;
    }
}

void RandomTwinkleEffect::changesMade()
{
    int oldTwinkleCount = twinkleCount;

    twinkleCount = percentOrValue(Settings::getInt("density"), numLEDs);

    Serial.printf("Twinkle count=%d/%d, delay=%d\n", twinkleCount, numLEDs, loopDelay);

    if (oldTwinkleCount > twinkleCount) {
        // We have to turn a bunch of LEDs off
        Serial.printf("Turning off excess LEDs\n");
        for (int i=twinkleCount; i<oldTwinkleCount; i++) {
            setLED(history[i], CRGB::Black);
            show();
            delay(loopDelay);
        }
        insert = 0;
    }
}

void RandomTwinkleEffect::loop()
{
    int ledNum = random(numLEDs);

    lightLed(ledNum, pickColour());

    show();
    delay(loopDelay);
}

CRGB RandomTwinkleEffect::pickColour()
{
    int r = random(15)<<4;
    int g = random(15)<<4;
    int b = random(15)<<4;

/*
    while ((r+g+b) < 32) {
        r = random(255);
        g = random(255);
        b = random(255);
    }
*/
    return CRGB(r, g, b);
}

void RandomTwinkleEffect::lightLed(int ledNum, CRGB col)
{
    if (history[insert] != -1) {
        //Serial.printf("Turn off LED %d\n", history[insert]);
        setLED(history[insert], CRGB::Black);
    }

    history[insert++] = ledNum;
    if (insert >= twinkleCount) {
        insert = 0;
    }

    //Serial.printf("Turn on LED %d\n", ledNum);
    setLED(ledNum, col);
}

void RandomTwinkleEffect::lightLed(int ledNum, int r, int g, int b)
{
    lightLed(ledNum, CRGB(r,g,b));
}