#pragma once

#include <AL/al.h>

#include "../Core.h"

namespace Crocodile
{
    class CROCODILE_API SoundSource
    {
    public:
        SoundSource();
        ~SoundSource();

        void play(const ALuint buffer);

    private:
        ALuint p_Source;
        float p_Pitch = 1.f;
        float p_Gain = 1.f;
        float p_Position[3] = {0, 0, 0};
        float p_Velocity[3] = {0, 0, 0};
        bool p_LoopSound = false;
        ALuint p_Buffer = 0;
    };
}