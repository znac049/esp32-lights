#include <Arduino.h>

#include "Settings.h"
#include "lights.h"

bool setupLEDs()
{
    switch(Settings::getInt("LEDOrder")) {
        default:
        case RGB:
            FastLED.addLeds<WS2812B, DATA_PIN1, RGB>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN2, RGB>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN3, RGB>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN4, RGB>(leds, MAX_LEDS);
            break;

        case RBG:
            FastLED.addLeds<WS2812B, DATA_PIN1, RBG>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN2, RBG>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN3, RBG>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN4, RBG>(leds, MAX_LEDS);
            break;
            
        case GBR:
            FastLED.addLeds<WS2812B, DATA_PIN1, GBR>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN2, GBR>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN3, GBR>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN4, GBR>(leds, MAX_LEDS);
            break;
            
        case GRB:
            FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN3, GRB>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN4, GRB>(leds, MAX_LEDS);
            break;
            
        case BGR:
            FastLED.addLeds<WS2812B, DATA_PIN1, BGR>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN2, BGR>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN3, BGR>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN4, BGR>(leds, MAX_LEDS);
            break;
            
        case BRG:
            FastLED.addLeds<WS2812B, DATA_PIN1, BRG>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN2, BRG>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN3, BRG>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN4, BRG>(leds, MAX_LEDS);
            break;            
    }

    FastLED.setBrightness(Settings::getInt("brightness"));

    return true;
}