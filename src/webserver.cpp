#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <ESPmDNS.h>
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

String decodeAction(const char *args) {
    String res = args;
    int query = res.indexOf('?');

    if (query != -1) {
        res = res.substring(0, query);
    }

    //Serial.println("Decoded action: " + res);
    return res;
}

String getArg(const char *args, const char *arg, const char *def) {
    String str = args;
    int sep = str.indexOf('?');

    if (sep == -1) {
        return def;
    }

    while (sep != -1) {
        String elem;
        int eq;
        String key, val;

        str = str.substring(sep+1);
        sep = str.indexOf('&');
        if (sep != -1) {
            elem = str.substring(0, sep);
        }
        else {
            elem = str;
        }

        eq = elem.indexOf('=');
        if (eq != -1) {
            key = elem.substring(0, eq);
            val = elem.substring(eq+1);

            if (key.equals(arg)) {
                return val;
            }
        }
        else {
            key = elem;
            if (key.equals(arg)) {
                return def;
            }
        }
    }

    return def;
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    String action;

    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        action = decodeAction((const char *) data);

        if (action.equals("toggle")) {
            ledState = !ledState;
            notifyClients();
        }
        else if (action.equals("save")) {
            char *args = (char *) data;

            if (Settings::setDeviceName(getArg(args, "name", "esp32"))) {
                Serial.println("Name has changed.");
                
                MDNS.setInstanceName(Settings::deviceName.c_str());
                dirty = true;
            }

            if (Settings::setNumLEDs(getArg(args, "numleds", ""))) {
                Serial.println("Number of LEDs has changed.");
                dirty = true;
            }

            if (Settings::setLEDOrder(getArg(args, "order", ""))) {
                Serial.println("LED Order has changed.");
                dirty = true;

                // It seems we have to reboot to change this :-(
                ESP.restart();
            }

            if (Settings::setPatternNumber(getArg(args, "pattern", ""))) {
                Serial.println("Patern Number changed.");
                effectNum = Settings::patternNumber;
                dirty = true;
            }

            if (Settings::setSpeed(getArg(args, "speed", "40"))) {
                Serial.println("Speed has changed.");
                dirty = true;
            }

            if (Settings::setBrightness(getArg(args, "brightness", "40"))) {
                Serial.println("Brightness has changed.");
                dirty = true;
            }

            if (Settings::setDensity(getArg(args, "density", "40"))) {
                Serial.println("Density has changed.");
                dirty = true;
            }

            if (Settings::setLoopDelay(getArg(args, "delay", "40"))) {
                Serial.println("Loop delay has changed.");
                dirty = true;
            }
        }
        else {
            Serial.println("Unknown command on websocket: " + action);
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
    if (macroName == "BLACKOUT") {
        if (!blackout) {
            res = "checked";
        }
        else {
            res = "";
        }
    } else if (macroName == "BRIGHTNESS") {
        res = Settings::brightness;
    } else if (macroName == "DELAY") {
        res = Settings::loopDelay;
    } else if (macroName == "DENSITY") {
        res = Settings::density;
    } else if (macroName == "DEVICEIP") {
        res = WiFi.localIP().toString();
    } else if (macroName == "DEVICENAME") {
        res = Settings::deviceName;
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
    else if (macroName == "LEDPIN") {
        res = ledPin;
    }
    else if (macroName == "NUMLEDS") {
        res = Settings::numLEDs;
    }
    else if (macroName == "PATTERNS") {
        res = "";
        for (int i=0; i<numEffects; i++) {
            res += "<option value=\"" + String(i) + "\"";
            if (effectNum == i) {
                res += " selected";
            }
            res += ">";
            res += effects[i]->getName();
            res += "</option>";
        }
    }
    else if (macroName == "SPEED") {
        res = Settings::speed;
    }
    else if(macroName == "STATE") {
        if (ledState){
            res = "ON";
        }
        else{
            res = "OFF";
        }
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