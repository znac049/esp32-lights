#include <WString.h>
#include <FastLED.h>

#include "defs.h"
#include "Settings.h"
#include "Effect.h"

#include "lights.h"

const char *Effect::getName(void) {
    return "base class";
}

void Effect::reset() 
{
}

void Effect::loop()
{
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
  if ((ledNum >= 0) && (ledNum < Settings::numLEDs)) {
    leds[ledNum] = colour;
  }
}

void Effect::setLED(int ledNum, int r, int g, int b) 
{
  if ((ledNum >= 0) && (ledNum < Settings::numLEDs)) {
    leds[ledNum].red = r;
    leds[ledNum].green = g;
    leds[ledNum].blue = b;
  }
}

void Effect::setAll(CRGB colour) 
{
  for (int i=0; i<Settings::numLEDs; i++) {
    leds[i] = colour;
  }
}

void Effect::setAll(int r, int g, int b) 
{
  setAll(CRGB(r, g, b));
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

/*

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  leds.setPixel(Pixel,red,green,blue);
  leds.show();
  delay(SpeedDelay);
  leds.setPixel(Pixel,0,0,0);
}



void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      leds.setPixel(i, *c, *(c+1), *(c+2));
    }
    leds.show();
    delay(SpeedDelay);
  }
}

void BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;
 
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
 
  for (int i = 0 ; i < BallCount ; i++) {  
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
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
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
 
    for (int i = 0 ; i < BallCount ; i++) {
      leds.setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    }
   
    leds.show();
    leds.setAll(0,0,0);
  }
}
*/