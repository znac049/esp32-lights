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
    int version;

    startPrefs();

    version = prefs.getInt("version", -1);

    Serial.print("nvram version: ");
    Serial.println(version);

    if (version != 3) {
        Serial.println("Settings reset to defaults in nvram.");

        prefs.clear();
        set("version", 3);

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

    return true;
}

bool Settings::saveRequired()
{
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
    startPrefs();
    String previousVal;
    
    property.toLowerCase();
    Serial.println(String("Settings::set - ") + property);

    previousVal = get(property);

    prefs.putString(property.c_str(), val);

    return !previousVal.equalsIgnoreCase(val);
}

bool Settings::set(String property, int val)
{
    startPrefs();
    int previousVal = getInt(property);

    property.toLowerCase();
    Serial.println(String("Settings::setInt - ") + property);

    prefs.putInt(property.c_str(), val);

    return (previousVal != val);
}

String Settings::get(String property)
{
    startPrefs();

    property.toLowerCase();
    //Serial.println(String("Settings::get - ") + property);

    return prefs.getString(property.c_str());
}

int Settings::getInt(String property)
{
    startPrefs();

    property.toLowerCase();
    //Serial.println(String("Settings::getInt - ") + property);

    return prefs.getInt(property.c_str());
}

