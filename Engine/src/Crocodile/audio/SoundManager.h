#pragma once

#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"

#include "../Core.h"

namespace Crocodile
{
    class CROCODILE_API SoundManager
    {
    public:
        SoundManager()
        {
            soundBuffer = SoundBuffer::get();
            soundDevice = SoundDevice::get();
            speaker = new SoundSource();
        };
        ~SoundManager()
        {
            delete speaker;
        };

        void play(std::string name)
        {
            ALuint sound = soundBuffer->getSound(name);
            speaker->play(sound);
        }

        SoundBuffer *soundBuffer = nullptr;

    private:
        SoundSource *speaker = nullptr;
        SoundDevice *soundDevice = nullptr;
    };
}
