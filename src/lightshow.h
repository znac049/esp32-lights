#ifndef _LIGHTSHOW_H_
#define _LIGHTSHOW_H_

#include "effects/Effect.h"

enum LightState {ST_IDLE, ST_RUNNING};

class LightShow {
    private:
        static const int MAX_EFFECTS=30;
        static Effect *effects[];
        static int numEffects;
        static int currentEffect;
        static LightState state;

        static int numLEDs;
        static int numStrings;
        static int loopDelay;

        void fade();
        void loop();
        void reset();

        bool isRunning() { return (state == ST_RUNNING); }
        bool changesMade = false;

    public:
        LightShow();
        void run();
        void changeState(LightState targetState);
        void setup();

        void addEffect(Effect *effect);

        void settingsChanged();

        int getNumLEDs() { return numLEDs; };
        int getnumStrings() { return numStrings; };
        int getLoopDelay() { return loopDelay; };
        int getCurrentEffectNum() { return currentEffect; };
};

#endif