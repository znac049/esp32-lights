#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <WString.h>
#include "lights.h"

class Settings {
    public:
        static String deviceName;
        static int numLEDs;
        static int LEDOrder;
        static int patternNumber;

        static bool load();
        static bool save();
};

#endif