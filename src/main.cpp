#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <FastLED.h>

#include "defs.h"
#include "SmartLED.h"
#include "effects/effects.h"
#include "Settings.h"

#include "lights.h"

extern JubileeEffect jub;
extern TorpedoEffect torp;
extern SimpleEffect simple;
extern RandomTwinkleEffect rte;
extern RainbowChaseEffect rainbow;
extern SnowSparkleEffect snow;
extern BouncingBallsEffect balls;

#define DATA_PIN 32

const char* ssid = "BT-TCCJ6M";
const char* password = "K69JyKkdNHm7ce";

int runningEffect=0;
int effectNum=0;

Effect *effects[] = {
    dynamic_cast<Effect*>(&jub),
    dynamic_cast<Effect*>(&torp),
    dynamic_cast<Effect*>(&simple),
    dynamic_cast<Effect*>(&rte),
    dynamic_cast<Effect*>(&rainbow), 
    dynamic_cast<Effect*>(&snow), 
    dynamic_cast<Effect*>(&balls), 
};

int numEffects = (sizeof(effects) / sizeof(Effect *));

bool ledState = 0;
const int ledPin = 21;
String hostname;


void setup()
{
    Serial.begin(115200);
    Settings::load();

    Serial.println("Device name: " + Settings::deviceName);
    blackout = false;
    
    switch(Settings::LEDOrder) {
        default:
        case RGB:
            FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, MAX_LEDS);
            break;

        case RBG:
            FastLED.addLeds<WS2812B, DATA_PIN, RBG>(leds, MAX_LEDS);
            break;
            
        case GBR:
            FastLED.addLeds<WS2812B, DATA_PIN, GBR>(leds, MAX_LEDS);
            break;
            
        case GRB:
            FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, MAX_LEDS);
            break;
            
        case BGR:
            FastLED.addLeds<WS2812B, DATA_PIN, BGR>(leds, MAX_LEDS);
            break;
            
        case BRG:
            FastLED.addLeds<WS2812B, DATA_PIN, BRG>(leds, MAX_LEDS);
            break;            
    }
    //FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, MAX_LEDS);
    FastLED.setBrightness(Settings::brightness);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(2000);
        Serial.println("Connecting to WiFi..");
    }

    if(!MDNS.begin(Settings::deviceName.c_str())) {
        Serial.println("Error starting mDNS");
        return;
    }

    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());

    setupWebserver();
    
    effectNum = Settings::patternNumber;

    Serial.print("Pattern Number=");
    Serial.println(effectNum);

    if (effectNum >= numEffects) {
        effectNum = 0;
        Settings::setPatternNumber(effectNum);
    }

    dirty = false;
    runningEffect = effectNum;
    effects[effectNum]->reset();
}

void loop() {
    cleanupWebsocketClients();
    digitalWrite(ledPin, ledState);

    if (!blackout) {
        if (dirty) {
            Serial.println("In main loop - changes to settings detected.");

            // Has the selected effect changed?
            if (runningEffect != effectNum) {
                effects[effectNum]->reset();
                runningEffect = effectNum;
            }

            FastLED.setBrightness(Settings::brightness);

            effects[effectNum]->changesMade();
            dirty = false;
        }
    }

    effects[effectNum]->loop();
}
