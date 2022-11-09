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

String Settings::deviceName;
int Settings::numLEDs;
int Settings::LEDOrder;
int Settings::patternNumber;
int Settings::speed;
int Settings::loopDelay;

bool Settings::load()
{
    Preferences prefs;
    int version;

    prefs.begin("chippers");
    version = prefs.getInt("version", -1);

    if (version == -1) {
        Serial.println("Settings reset to defaults in nvram.");

        prefs.clear();
        prefs.putInt("version", 1);

        Settings::deviceName = "esp32";
        Settings::numLEDs = MAX_LEDS;
        Settings::LEDOrder = RGB;
        Settings::patternNumber = 0;
        Settings::speed = 30;
        Settings::loopDelay = 30;

        Settings::save();
    }
    else {
        Settings::deviceName = prefs.getString("deviceName", "esp32");
        Settings::numLEDs = prefs.getInt("numLEDs", MAX_LEDS);
        Settings::LEDOrder = prefs.getInt("LEDOrder", RGB);
        Settings::patternNumber = prefs.getInt("patternNumber", 0);
        Settings::speed = prefs.getInt("speed", RGB);
        Settings::loopDelay = prefs.getInt("loopDelay", RGB);
    }

    prefs.end();

    Serial.println("Settings loaded from nvram:");
    Serial.println(" deviceName: " + Settings::deviceName);
    Serial.println(" Num LEDS: " + String(Settings::numLEDs));
    Serial.println(" LED Order: " + String(Settings::LEDOrder));
    Serial.println(" Pattern #: " + String(Settings::patternNumber));
    Serial.println(" Speed: " + String(Settings::speed));
    Serial.println(" Loop Delay: " + String(Settings::loopDelay));
    
    return true;
}

bool Settings::save()
{
    Preferences prefs;

    prefs.begin("chippers");
    prefs.putString("deviceName", Settings::deviceName);
    prefs.putInt("numLEDs", Settings::numLEDs);
    prefs.putInt("LEDOrder", Settings::LEDOrder);
    prefs.putInt("patternNumber", Settings::patternNumber);
    prefs.putInt("speed", Settings::speed);
    prefs.putInt("loopDelay", Settings::loopDelay);
    prefs.end();

    Serial.println("Settings saved to nvram.");

    return true;
}

bool Settings::setDeviceName(String newName) {
    if (!newName.equals(Settings::deviceName)) {
        Settings::deviceName = newName;
        Settings::save();

        return true;
    }

    return false;
}

bool Settings::setNumLEDs(int newval) {
    if (newval != Settings::numLEDs) {
        Settings::numLEDs = newval;
        Settings::save();

        return true;
    }   

    return false;
}

bool Settings::setNumLEDs(String newval) {
    return Settings::setNumLEDs(newval.toInt());
}

bool Settings::setLEDOrder(int newval) {
    if (newval != Settings::LEDOrder) {
        Settings::LEDOrder = newval;
        Settings::save();

        return true;
    }   

    return false;
}

bool Settings::setLEDOrder(String newval) {
    return Settings::setLEDOrder(newval.toInt());
}

bool Settings::setPatternNumber(int newval) {
    if (newval != Settings::patternNumber) {
        Settings::patternNumber = newval;
        Settings::save();

        return true;
    }   

    return false;
}

bool Settings::setPatternNumber(String newval) {
    return Settings::setPatternNumber(newval.toInt());
}

bool Settings::setSpeed(int newval) {
    if (newval != Settings::speed) {
        Settings::speed = newval;
        Settings::save();

        return true;
    }   

    return false;
}

bool Settings::setSpeed(String newval) {
    return Settings::setSpeed(newval.toInt());
}

bool Settings::setLoopDelay(int newval) {
    if (newval != Settings::loopDelay) {
        Settings::loopDelay = newval;
        Settings::save();

        return true;
    }   

    return false;
}

bool Settings::setLoopDelay(String newval) {
    return Settings::setLoopDelay(newval.toInt());
}

