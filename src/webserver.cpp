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
        //Serial.println("getArg(def): " + String(def));
        return def;
    }

    while (sep != -1) {
        String elem;
        int eq;
        String key, val;

        str = str.substring(sep+1);

        //Serial.println("Args: " + str);

        sep = str.indexOf('&');
        if (sep != -1) {
            elem = str.substring(0, sep);
        }
        else {
            elem = str;
        }

        //Serial.println("Elem=" + elem);

        eq = elem.indexOf('=');
        if (eq != -1) {
            key = elem.substring(0, eq);
            val = elem.substring(eq+1);

            //Serial.println("k=" + key + ", v=" + val);

            if (key.equals(arg)) {
                //Serial.println("Found arg " + key + ": " + val);
                return val;
            }
        }
        else {
            key = elem;
            //Serial.println("k=" + key);
            if (key.equals(arg)) {
                //Serial.println("Found arg (no value)" + key + ": " + def + "(default)");
                return def;
            }
        }
    }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    String action;

    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        //Serial.print("ws got: ");
        //Serial.println((char*) data);

        action = decodeAction((const char *) data);
        //Serial.println("Action: " + action);

        if (action.equals("toggle")) {
            ledState = !ledState;
            notifyClients();
        }
        else if (action.equals("save")) {
            String name = getArg((const char *) data, "name", "esp32");
            String numleds = getArg((const char *) data, "numleds", "");
            String order = getArg((const char *) data, "order", "");
            String pattern = getArg((const char *) data, "pattern", "");

            Serial.println("New device name: " + name);
            Serial.println("num LEDs: " + numleds);
            Serial.println("LED order: " + order);
            Serial.println("Pattern #: " + pattern);
            Serial.println("-------------------");

            if (Settings::setDeviceName(name)) {
                Serial.println("Name has changed.");
            }

            if (Settings::setNumLEDs(numleds)) {
                Serial.println("Number of LEDs has changed.");
            }

            if (Settings::setLEDOrder(order)) {
                Serial.println("LED Order has changed.");
            }

            if (Settings::setPatternNumber(pattern)) {
                Serial.println("Patern Number changed.");
                effectNum = Settings::patternNumber;
            }
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
        res = Settings::numLEDs;
    }
    else if (macroName == "LEDPIN") {
        res = ledPin;
    }
    else if (macroName == "PATTERNS") {
        res = "";
        for (int i=0; i<numEffects; i++) {
            res += "<option value=\"" + String(i) + "\"";
            if (effectNum == i) {
                res += " selected";
            }
            res += ">";
            res += effects[i].effect->getName();
            res += "</option>";
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