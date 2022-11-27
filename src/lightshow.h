#ifndef _LIGHTSHOW_H_
#define _LIGHTSHOW_H_

#include "effects/Effect.h"

class LightShow {
    private:
        static const int MAX_EFFECTS=30;
        static Effect *effects[];
        static int numEffects;
        static int currentEffect;

    public:
        int numLEDs;

        LightShow();
        void run();

        void addEffect(Effect *effect);
};

#endif