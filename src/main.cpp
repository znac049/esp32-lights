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

int runningEffect=0;
int effectNum=0;

Effect *effects[] = {
    dynamic_cast<Effect*>(&rte),
    dynamic_cast<Effect*>(&jub),
    dynamic_cast<Effect*>(&torp),
    dynamic_cast<Effect*>(&simple),
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
    int wifiOk;
    int wifiTries = 0;

    Serial.begin(115200);
    Settings::loadRequired();

    Serial.println("Device name: " + Settings::get("deviceName"));
    blackout = false;
    
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
    //FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, MAX_LEDS);
    FastLED.setBrightness(Settings::getInt("brightness"));

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Connect to Wi-Fi
    WiFi.begin(Settings::get("ssid").c_str(), Settings::get("wifiPassword").c_str());
    
    wifiOk = WiFi.status();
    while ((wifiOk != WL_CONNECTED) && (wifiTries < 10)) {
        delay(1000);
        wifiTries++;
        Serial.print("Connecting to WiFi... attempt ");
        Serial.println(wifiTries);
        wifiOk = WiFi.status();
    }

    if (wifiOk == WL_CONNECTED) {
        if(!MDNS.begin(Settings::get("deviceName").c_str())) {
            Serial.println("Error starting mDNS");
            return;
        }

        // Print ESP Local IP Address
        Serial.println(WiFi.localIP());
    }
    else {
        // Set myself up as an access point
        Serial.println("Creating local access point...");
        WiFi.mode(WIFI_AP);
        WiFi.softAP((Settings::get("ssid") + ".ap").c_str(), "12345678");
        Serial.println(WiFi.softAPIP());
    }

    Serial.println(WiFi.macAddress());

    setupWebserver();
    
    effectNum = Settings::getInt("patternNumber");

    Serial.print("Pattern Number=");
    Serial.println(effectNum);

    if (effectNum >= numEffects) {
        effectNum = 0;
        Settings::set("patternNumber", effectNum);
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

            FastLED.setBrightness(Settings::getInt("brightness"));

            effects[effectNum]->changesMade();
            dirty = false;
        }
    }

    effects[effectNum]->loop();
}
