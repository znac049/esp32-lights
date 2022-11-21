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

            if (Settings::set("deviceName", getArg(args, "name", DEFAULT_NAME))) {
                MDNS.setInstanceName(Settings::get("deviceName").c_str());
                dirty = true;
            }

            if (Settings::set("numLEDs", getArg(args, "numleds", "").toInt())) {
                dirty = true;
            }

            if (Settings::set("LEDOrder", getArg(args, "order", "").toInt())) {
                // It seems we have to reboot to change this :-(
                ESP.restart();
            }

            if (Settings::set("patternNumber", getArg(args, "pattern", "").toInt())) {
                effectNum = Settings::getInt("patternNumber");
                dirty = true;
            }

            if (Settings::set("speed", getArg(args, "speed", "40").toInt())) {
                dirty = true;
            }

            if (Settings::set("brightness", getArg(args, "brightness", "40").toInt())) {
                dirty = true;
            }

            if (Settings::set("density", getArg(args, "density", "40").toInt())) {
                dirty = true;
            }

            if (Settings::set("loopDelay", getArg(args, "delay", "40").toInt())) {
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

String getOrderOption(String text, int val) {
    String res = "<option ";

    res = res + "value=\"";
    res += val;
    res += "\"";
    if (Settings::getInt("LEDOrder") == val) {
        res += " selected";
    }
    res += ">" + text + "</option>";

    return res;
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
        res = Settings::getInt("brightness");
    } else if (macroName == "DELAY") {
        res = Settings::getInt("loopDelay");
    } else if (macroName == "DENSITY") {
        res = Settings::getInt("density");
    } else if (macroName == "DEVICEIP") {
        res = WiFi.localIP().toString();
    } else if (macroName == "DEVICENAME") {
        res = Settings::get("deviceName");
    }
    else if (macroName == "LEDORDER") {
        res = getOrderOption("RGB", RGB);
        res += getOrderOption("RBG", RBG);
        res += getOrderOption("GBR", GBR);
        res += getOrderOption("GRB", GRB);
        res += getOrderOption("BRG", BRG);
        res += getOrderOption("BGR", BGR);
    }
    else if (macroName == "LEDPIN") {
        res = ledPin;
    }
    else if (macroName == "NUMLEDS") {
        res = Settings::getInt("numLEDs");
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
        res = Settings::getInt("speed");
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