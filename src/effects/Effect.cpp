#include <WString.h>
#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"

#include "lights.h"

const char *Effect::getName(void) {
    return "base class";
}

void Effect::changesMade()
{
    Serial.println("Changes have been made!");
}

void Effect::reset(int _numLEDs, int _numStrings, int _loopDelay) 
{
    Serial.printf("Reset the effect: %d, %d, %d\n", _numLEDs, _numStrings, _loopDelay);

    numLEDs = _numLEDs;
    numStrings = _numStrings;
    stringLen = numLEDs / numStrings;
    loopDelay = _loopDelay;
}

void Effect::loop()
{
    Serial.println("Empty effect loop!");
}

void Effect::clearAll()
{
    setAll(CRGB::Black);
}

void Effect::show()
{
    FastLED.show();
}

void Effect::setLED(int ledNum, CRGB colour) 
{
    //Serial.printf("set: %d->#%06x (%d)\n", ledNum, (colour.r<<16)|(colour.g<<8)|colour.b, numLEDs);

    if ((ledNum >= 0) && (ledNum < numLEDs)) {
        leds[ledNum] = colour;
    }
}

void Effect::setLED(int stringNum, int ledNum, CRGB colour) 
{
    setLED((stringNum * stringLen) + ledNum, colour);
}

void Effect::setLED(int ledNum, int r, int g, int b) 
{
    if ((ledNum >= 0) && (ledNum < numLEDs)) {
        leds[ledNum].red = r;
        leds[ledNum].green = g;
        leds[ledNum].blue = b;
    }
}

void Effect::setAll(CRGB colour) 
{
    for (int i=0; i<numLEDs; i++) {
        leds[i] = colour;
    }
}

void Effect::setAll(int r, int g, int b) 
{
    setAll(CRGB(r, g, b));
}

void Effect::setString(int stringNum, CRGB colour)
{
  if ((stringNum >= 0) && (stringNum < numStrings)) {
    for (int i=0; i<stringLen; i++) {
      setLED(stringNum, i, colour);
    }
  }
}

byte *Effect::Wheel(byte WheelPos) 
{
    static byte c[3];
 
    if (WheelPos < 85) {
        c[0]=WheelPos * 3;
        c[1]=255 - WheelPos * 3;
        c[2]=0;
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        c[0]=255 - WheelPos * 3;
        c[1]=0;
        c[2]=WheelPos * 3;
    } else {
        WheelPos -= 170;
        c[0]=0;
        c[1]=WheelPos * 3;
        c[2]=255 - WheelPos * 3;
    }

    return c;
}

int Effect::percentOrValue(int pcnt, int val)
{
    if (pcnt > 0) {
        return pcnt;
    }

    return (-pcnt * val)/100;
}
