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
    //ws.textAll(String(ledState));
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

        Serial.println(String("Got websocket action: ") + action);
        if (action.equals("save")) {
            char *args = (char *) data;
            String val;

            if (Settings::set("deviceName", getArg(args, "name", DEFAULT_NAME))) {
                MDNS.setInstanceName(Settings::get("deviceName").c_str());
                dirty = true;
            }

            if (Settings::set("numLEDs", getArg(args, "numleds", "").toInt())) {
                dirty = true;
            }

            if (Settings::set("ssid", getArg(args, "ssid", ""))) {
                dirty = true;
            }

            // Ignore empty password
            val = getArg(args, "pass", "");
            if (!val.isEmpty() && Settings::set("wifiPassword", val)) {
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
        else if (action.equalsIgnoreCase("lightson")) {
            if (blackout) {
                Serial.println("Turning the lights back on");
            }
            blackout = false;
        }
        else if (action.equalsIgnoreCase("lightsoff")) {
            if (!blackout) {
                Serial.println("Turning the lights off");
            }
            blackout = true;
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
            Serial.printf("Ignoring websocket PONG event\n");
            break;

        case WS_EVT_ERROR:
            Serial.printf("Ignoring websocket ERROR event\n");
            break;
        
        default:
            Serial.printf("Unknown websocket event received\n");
            break;
    }
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
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