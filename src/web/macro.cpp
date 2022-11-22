#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>

#include "Settings.h"
#include "lights.h"

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
    else if (macroName == "SSID") {
        res = Settings::get("ssid");
    }
    else {
        Serial.print("Unknown macro: ");
        Serial.println(macroName);
    }
 
    return res;
}
