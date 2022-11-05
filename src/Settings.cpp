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

        Settings::save();
    }
    else {
        Settings::deviceName = prefs.getString("deviceName", "esp32");
        Settings::numLEDs = prefs.getInt("numLEDs", MAX_LEDS);
        Settings::LEDOrder = prefs.getInt("LEDOrder", RGB);
        Settings::patternNumber = prefs.getInt("patternNumber", 0);
    }

    prefs.end();

    Serial.println("Settings loaded from nvram.");

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
    prefs.end();

    Serial.println("Settings saved to nvram.");

    return true;
}