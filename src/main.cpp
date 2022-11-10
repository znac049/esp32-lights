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
};

int numEffects = (sizeof(effects) / sizeof(Effect *));

bool ledState = 0;
const int ledPin = 21;
String hostname;

/*int getColour(CRGB colour)
{
  return 0x10000000 | colour.r<<16 | colour.g<<8 | colour.b;
}*/

void setup()
{
  Serial.begin(115200);
  Settings::load();

  Serial.println("Device name: " + Settings::deviceName);
  for (int i=0; i<4; i++) {
    Serial.println(" FX: " + String(effects[i]->getName()));
  }
  
  //FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, MAX_LEDS);
  FastLED.setBrightness(16);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  for (int i=0; i<numEffects; i++) {
    Serial.println(effects[i]->getName());
  }
  
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

  runningEffect = effectNum;

  Serial.print("Starting pattern: ");
  Serial.println(effects[effectNum]->getName());
  effects[effectNum]->reset();
}

void loop() {
  cleanupWebsocketClients();
  digitalWrite(ledPin, ledState);

  // Have the selected effect changed?
  if (runningEffect != effectNum) {
    effects[effectNum]->reset();
    runningEffect = effectNum;
  }

  effects[effectNum]->loop();
}
