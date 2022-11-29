#include <Arduino.h>

#include "Settings.h"
#include "lights.h"

bool setupLEDs()
{
    int numLEDs = Settings::getInt("numLEDs");
    int numStrings = Settings::getInt("numStrings");
    bool parallel = (Settings::getInt("parallel") == 0);
    int order = Settings::getInt("LEDOrder");
    int stringLen = numLEDs;

    if (parallel) {
        stringLen = numLEDs / numStrings;
        numLEDs = stringLen;

        Serial.printf("Setting string length to %d and numLEDs to %d (for parallel use)\n", stringLen, numLEDs);
    }
    else {
        Serial.printf("Setting each string length to %d and numLEDs to %d (for sequential use)\n", stringLen, numLEDs);
    }
    
    switch(order) {
        default:
        case RGB:
            if (parallel) {
                FastLED.addLeds<WS2812B, DATA_PIN1, RGB>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN2, RGB>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN3, RGB>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN4, RGB>(leds, MAX_LEDS);
            }
            else {
                FastLED.addLeds<WS2812B, DATA_PIN1, RGB>(leds, 0, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN2, RGB>(leds, stringLen, MAX_LEDS-stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN3, RGB>(leds, stringLen*2, MAX_LEDS-(stringLen*2));
                FastLED.addLeds<WS2812B, DATA_PIN4, RGB>(leds, stringLen*3, MAX_LEDS-(stringLen*3));
            }
            break;

        case RBG:
            if (parallel) {
                FastLED.addLeds<WS2812B, DATA_PIN1, RBG>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN2, RBG>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN3, RBG>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN4, RBG>(leds, MAX_LEDS);
            } 
            else {
                FastLED.addLeds<WS2812B, DATA_PIN1, RBG>(leds, 0, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN2, RBG>(leds, stringLen, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN3, RBG>(leds, stringLen*2, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN4, RBG>(leds, stringLen*3, stringLen);
            }
            break;
                
        case GBR:
            if (parallel) {
                FastLED.addLeds<WS2812B, DATA_PIN1, GBR>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN2, GBR>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN3, GBR>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN4, GBR>(leds, MAX_LEDS);
            }
            else {
                FastLED.addLeds<WS2812B, DATA_PIN1, GBR>(leds, 0, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN2, GBR>(leds, stringLen, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN3, GBR>(leds, stringLen*2, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN4, GBR>(leds, stringLen*3, stringLen);
            }
            break;
                
        case GRB:
            if (parallel) {
                FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN3, GRB>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN4, GRB>(leds, MAX_LEDS);
            }
            else {
                FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds, 0, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds, stringLen, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN3, GRB>(leds, stringLen, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN4, GRB>(leds, stringLen, stringLen);
            }
            break;
                
        case BGR:
            if (parallel) {
                FastLED.addLeds<WS2812B, DATA_PIN1, BGR>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN2, BGR>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN3, BGR>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN4, BGR>(leds, MAX_LEDS);
            }
            else {
                FastLED.addLeds<WS2812B, DATA_PIN1, BGR>(leds, 0, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN2, BGR>(leds, stringLen, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN3, BGR>(leds, stringLen*2, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN4, BGR>(leds, stringLen*3, stringLen);
            }
            break;
                
        case BRG:
            if (parallel) {
                FastLED.addLeds<WS2812B, DATA_PIN1, BRG>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN2, BRG>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN3, BRG>(leds, MAX_LEDS);
                FastLED.addLeds<WS2812B, DATA_PIN4, BRG>(leds, MAX_LEDS);
            }
            else {
                FastLED.addLeds<WS2812B, DATA_PIN1, BRG>(leds, 0, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN2, BRG>(leds, stringLen, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN3, BRG>(leds, stringLen*2, stringLen);
                FastLED.addLeds<WS2812B, DATA_PIN4, BRG>(leds, stringLen*3, stringLen);
            }
            break;            
    }

    FastLED.setBrightness(Settings::getInt("brightness"));

    return true;
}