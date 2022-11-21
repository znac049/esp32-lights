#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <Preferences.h>

class Settings {
    private:
        static bool prefsStarted;
        static Preferences prefs;

        static void startPrefs();

    
        static String deviceName;
        
        static String ssid;
        static String wifiPassword;

        static String webPassword;

        static int numLEDs;
        static int LEDOrder;
        static int patternNumber;
        static int speed;
        static int loopDelay;
        static int density;
        static int brightness;

        static bool saveRequired();

    public:
        static bool loadRequired();

        static bool set(String property, String val);
        static bool set(String property, int val);

        static String get(String property);
        static int getInt(String property);
};

#endif