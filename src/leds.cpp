#include <Arduino.h>

#include "Settings.h"
#include "lights.h"

bool setupLEDs()
{
    int numLEDS = Settings::getInt("numLEDs");
    int numStrings = Settings::getInt("numStrings");
    int parallel = Settings::getInt("parallel");
    int firstLED = 0;
    int order = Settings::getInt("LEDOrder");
    int increment = 0;

    CRGB *string = &leds[firstLED];

    //Serial.printf("String %d starts at %d\n", i, firstLED);

    switch(order) {
        default:
        case RGB:
            //FastLED.addLeds<WS2812B, DATA_PIN1, RGB>(leds, MAX_LEDS);
            FastLED.addLeds<WS2812B, DATA_PIN1, RGB>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN2, RGB>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN3, RGB>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN4, RGB>(string, MAX_LEDS);
            string += increment;
            break;

        case RBG:
            FastLED.addLeds<WS2812B, DATA_PIN1, RBG>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN2, RBG>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN3, RBG>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN4, RBG>(string, MAX_LEDS);
            string += increment;
            break;
                
        case GBR:
            FastLED.addLeds<WS2812B, DATA_PIN1, GBR>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN2, GBR>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN3, GBR>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN4, GBR>(string, MAX_LEDS);
            string += increment;
            break;
                
        case GRB:
            FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN3, GRB>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN4, GRB>(string, MAX_LEDS);
            string += increment;
            break;
                
        case BGR:
            FastLED.addLeds<WS2812B, DATA_PIN1, BGR>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN2, BGR>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN3, BGR>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN4, BGR>(string, MAX_LEDS);
            string += increment;
            break;
                
        case BRG:
            FastLED.addLeds<WS2812B, DATA_PIN1, BRG>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN2, BRG>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN3, BRG>(string, MAX_LEDS);
            string += increment;
            FastLED.addLeds<WS2812B, DATA_PIN4, BRG>(string, MAX_LEDS);
            string += increment;
            break;            
    }

    FastLED.setBrightness(Settings::getInt("brightness"));

    return true;
}