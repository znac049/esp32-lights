#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <FastLED.h>

#include "defs.h"
#include "SmartLED.h"
#include "effects/Effect.h"
#include "effects/Jubilee.h"
#include "effects/Torpedo.h"
#include "Settings.h"

#include "lights.h"

extern JubileeEffect jub;
extern TorpedoEffect torp;

#define DATA_PIN 32

const char* ssid = "BT-TCCJ6M";
const char* password = "K69JyKkdNHm7ce";

struct Effects effects[2];

bool ledState = 0;
const int ledPin = 21;
String hostname;

int getColour(CRGB colour)
{
  return 0x10000000 | colour.r<<16 | colour.g<<8 | colour.b;
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  Settings::load();
  effects[0].name = jub.getName();
  effects[0].effect = dynamic_cast<Effect*>(&jub);
  effects[1].name = torp.getName();
  effects[1].effect = dynamic_cast<Effect*>(&torp);

  Serial.println("Device name: " + Settings::deviceName);

  //FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, MAX_LEDS);
  FastLED.setBrightness(16);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println(jub.getName());
  Serial.println(torp.getName());
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  if(!MDNS.begin(Settings::deviceName.c_str())) {
     Serial.println("Error starting mDNS");
     return;
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  setupWebserver();
  
  jub.reset();
}

void loop() {
  cleanupWebsocketClients();
  digitalWrite(ledPin, ledState);

  jub.loop();
}

int setLED(int offset, CRGB colour) {
  offset = offset%MAX_LEDS;
  leds[offset] = colour;

  return offset;
}