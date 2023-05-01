#pragma once

#include <AL/alc.h>
#include <iostream>

#include "../Core.h"

namespace Crocodile
{
    class CROCODILE_API SoundDevice
    {
    public:
        static SoundDevice *get()
        {
            static SoundDevice *snd_manager = new SoundDevice();
            return snd_manager;
        }

    private:
        SoundDevice()
        {
            init();
        }
        ~SoundDevice()
        {
            alcCloseDevice(p_ALCDevice);
        }

        void init()
        {
            p_ALCDevice = alcOpenDevice(nullptr);
            if (!p_ALCDevice)
                throw("failed to get sound device");

            p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
            if (!p_ALCContext)
                throw("failed to get sound context");

            if (!alcMakeContextCurrent(p_ALCContext))
                throw("failed to make context current");
        }

        ALCdevice *p_ALCDevice;
        ALCcontext *p_ALCContext;
    };
}
