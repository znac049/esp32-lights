#ifndef _SETTINGS_H_
#define _SETTINGS_H_

class Settings {
    public:
        static String deviceName;
        static int numLEDs;
        static int LEDOrder;
        static int patternNumber;

        static bool load();
        static bool save();
        static bool setDeviceName(String newName);
        static bool setNumLEDs(String newval);
        static bool setNumLEDs(int newval);
        static bool setLEDOrder(String newval);
        static bool setLEDOrder(int newval);
        static bool setPatternNumber(String newval);
        static bool setPatternNumber(int newval);
};

#endif