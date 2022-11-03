#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Preferences.h>
#include "lights.h"
#include "FastLED.h"
#include "SmartLED.h"
#include "effects/Jubilee.h"
#include "effects/Torpedo.h"

#define DATA_PIN 32

const char* ssid = "BT-TCCJ6M";
const char* password = "K69JyKkdNHm7ce";

CRGB leds[MAX_LEDS];

JubileeEffect jub;
TorpedoEffect torp;

Preferences prefs;

int getColour(CRGB colour) {
  return 0x10000000 | colour.r<<16 | colour.g<<8 | colour.b;
}

bool ledState = 0;
const int ledPin = 21;
String hostname;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

#include "html.h"

void notifyClients() {
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.print("ws got: ");
    Serial.println((char*) data);
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  } else if (var == "DEVICENAME") {
    return hostname;
  }
  else if (var == "DEVICEIP") {
    return WiFi.localIP().toString();
  }
  else if (var == "LEDORDER") {
    return "<option>RGB</option>"
           "<option>RBG</option>"
           "<option>GRB</option>"
           "<option>GBR</option>"
           "<option>BRG</option>"
           "<option>BGR</option>";
  }
  else if (var == "NUMLEDS") {
    char tmpstr[8];
    return itoa(MAX_LEDS, tmpstr, 10);
  }

  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  prefs.begin("chippers");
  hostname = prefs.getString("hostname", "");
  if (hostname.length() == 0) {
    Serial.println("Hostname not set");
    hostname = "esp32";
  }

  Serial.println("Using hostname: " + hostname);

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

  if(!MDNS.begin(hostname.c_str())) {
     Serial.println("Error starting mDNS");
     return;
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start ElegantOTA
  AsyncElegantOTA.begin(&server);
  // Start server
  server.begin();

  jub.reset();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
  //Serial.print(".");

  jub.loop();
}

int setLED(int offset, CRGB colour) {
  offset = offset%MAX_LEDS;
  leds[offset] = colour;

  return offset;
}