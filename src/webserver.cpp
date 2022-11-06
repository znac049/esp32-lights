#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Preferences.h>
#include <FastLED.h>

#include "defs.h"
#include "effects/Effect.h"
#include "lights.h"
#include "Settings.h"
#include "html.html"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyClients()
{
    ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
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
             void *arg, uint8_t *data, size_t len)
{
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
        default:
            break;
    }
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

String lookupMacro(const String& macroName)
{
    String res = "";

    Serial.println(macroName);
    if(macroName == "STATE"){
        if (ledState){
            res = "ON";
        }
        else{
            res = "OFF";
        }
    } else if (macroName == "DEVICENAME") {
        res = Settings::deviceName;
    }
    else if (macroName == "DEVICEIP") {
        res = WiFi.localIP().toString();
    }
    else if (macroName == "LEDORDER") {
        res = "<option value=\"";
        res += RGB;
        res += "\">RGB</option><option value=\"";
        res += RBG;
        res += "\">RBG</option><option value=\"";
        res += GRB;
        res += "\">GRB</option><option value=\"";
        res += GBR;
        res += "\">GBR</option><option value=\"";
        res += BRG;
        res += "\">BRG</option><option value=\"";
        res += BGR;
        res += "\">BGR</option>";
    }
    else if (macroName == "NUMLEDS") {
        res = MAX_LEDS;
//        char tmpstr[8];
//        return itoa(MAX_LEDS, tmpstr, 10);
    }
    else if (macroName == "LEDPIN") {
        res = ledPin;
//        char tmpstr[8];
//        return  itoa(ledPin, tmpstr, 10);
    }
    else if (macroName == "PATTERNS") {
        res = "<option value=\"0\">Jubilee</option>";
        res += "<option value=\"1\">Torpedo</option>";
        res += "<option value=\"2\">Simple</option>";
    }

    return res;
}

void setupWebserver()
{
    initWebSocket();

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, lookupMacro);
    });

    AsyncElegantOTA.begin(&server);
    server.begin();
}

void cleanupWebsocketClients()
{
    ws.cleanupClients();
}