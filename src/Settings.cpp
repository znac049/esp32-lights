#include <Preferences.h>

#include "FASTLED.h"
#include <WString.h>

#include "defs.h"
#include "SmartLED.h"
#include "effects/Effect.h"
#include "effects/Jubilee.h"
#include "effects/Torpedo.h"
#include "Settings.h"
#include "lights.h"

bool Settings::prefsStarted = false;
Preferences Settings::prefs;

String Settings::deviceName;

String Settings::ssid;
String Settings::wifiPassword;

String Settings::webPassword;

int Settings::numLEDs;
int Settings::LEDOrder;
int Settings::patternNumber;
int Settings::speed;
int Settings::loopDelay;
int Settings::density = -25;
int Settings::brightness = 16;

void Settings::initSettings()
{
    Serial.println("Settings reset to defaults in nvram.");

    prefs.clear();
    set("version", 4);

    set("deviceName", DEFAULT_NAME);

    set("ssid", DEFAULT_SSID);
    set("wifiPassword", DEFAULT_WIFI_PASSWORD);

    set("webPassword", DEFAULT_WEB_PASSWORD);
        
    set("numLEDs", MAX_LEDS);
    set("LEDOrder", RGB);
    set("patternNumber", 0);
    set("speed", DEFAULT_SPEED);
    set("loopDelay", DEFAULT_LOOP_DELAY);
    set("brightness", DEFAULT_BRIGHTNESS);
    set("density", DEFAULT_DENSITY);

    set("numStrings", DEFAULT_NUM_STRINGS);
    set("mirroredStrings", DEFAULT_MIRRORED_STRINGS);

    Serial.println("Required settings saved to nvram.");
}

void Settings::startPrefs()
{
    if (!prefsStarted) {
        Serial.println("Starting Prefs...");
        prefs.begin("chippers");
        prefsStarted = true;
    }
}

bool Settings::loadRequired()
{
    int version = -1;
    int resetRequired = digitalRead(RESET_NVRAM_PIN);

    Serial.printf("DigitalRead(%d) = %d\n", RESET_NVRAM_PIN, resetRequired);
    resetRequired = 1;

    startPrefs();

    if (resetRequired == 0) {
        Serial.println("Reset NVRAM jumper present. Re-initialising NVRAM...");
    }
    else {
        if (prefs.isKey("version")) {
            version = prefs.getInt("version", -1);
        }

        Serial.printf("nvram version: %d\n", version);
    }

    if ((resetRequired == 0) || (version != CURRENT_VERSION)) {
        initSettings();
    }

    return true;
}

bool Settings::saveRequired()
{
    set("version", CURRENT_VERSION);

    set("deviceName", deviceName);

    set("ssid", ssid);
    set("wifiPassword", wifiPassword);

    set("webPassword", webPassword);

    set("numLEDs", numLEDs);
    set("LEDOrder", LEDOrder);
    set("patternNumber", patternNumber);
    set("speed", speed);
    set("loopDelay", loopDelay);
    set("brightness", brightness);
    set("density", density);

    Serial.println("Required settings saved to nvram.");

    return true;
}

bool Settings::set(String property, String val)
{
    String previousVal;
    const char *prop;
    bool changed = false;
 
    startPrefs();
    
    property.toLowerCase();
    prop = property.c_str();

    // Does the key alredy exist?
    if (prefs.isKey(prop)) {
        PreferenceType keyType = prefs.getType(prop);

        if (keyType != PT_STR) {
            Serial.printf("Warning: existing key '%s'is not of type STRING.", prop);
        }

        previousVal = get(property);
        if (!previousVal.equals(val)) {
            Serial.printf("Changing existing key '%s' from '%s' to '%s'\n", prop, previousVal.c_str(), val.c_str());
            prefs.putString(prop, val);
            
            changed = true;
        }
    }
    else {
        Serial.printf("Setting new key '%s' to '%s'\n", property.c_str(), val.c_str());
        prefs.putString(prop, val);

        changed = true;
    }

    return changed;
}

bool Settings::set(String property, int val)
{
    int previousVal;
    const char *prop;
    bool changed = false;

    startPrefs();

    property.toLowerCase();
    prop = property.c_str();

    // Does the key alredy exist?
    if (prefs.isKey(prop)) {
        PreferenceType keyType = prefs.getType(prop);

        if (keyType != PT_I32) {
            Serial.printf("Warning: existing key '%s' is not of type INT32.", prop);
        }

        previousVal = getInt(property);
        if (previousVal != val) {
            Serial.printf("Changing existing key '%s' from %d to %d\n", prop, previousVal, val);
            prefs.putInt(prop, val);
            
            changed = true;
        }
    }
    else {
        Serial.printf("Setting new key '%s' to %d\n", prop, val);
        prefs.putInt(prop, val);

        changed = true;
    }

    return changed;
}

bool Settings::set(String property, char *val)
{
    return set(property, String(val));
}

String Settings::get(String property)
{
    String val;
    const char *prop;

    startPrefs();

    property.toLowerCase();
    prop = property.c_str();

    if (!prefs.isKey(prop)) {
        Serial.printf("Attempting to read a non-existant key '%s'\n", prop);
    }
    else {
        val = prefs.getString(prop);
        Serial.printf("Read '%s' as '%s'\n", prop, val);
    }

    return val;
}

int Settings::getInt(String property)
{
    startPrefs();

    property.toLowerCase();
    //Serial.println(String("Settings::getInt - ") + property);

    return prefs.getInt(property.c_str());
}

