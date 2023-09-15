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
            for (unsigned int i = 0; i < nSources; i++)
            {
                SoundSource *source = new SoundSource();
                sources.push_back(source);
            }
        };
        ~SoundManager()
        {
            for (unsigned int i = 0; i < nSources; i++)
            {
                SoundSource *source = sources[i];
                delete source;
            }
            sources.clear();
        };

        void play(std::string name, unsigned int source)
        {
            ALuint sound = soundBuffer->getSound(name);
            sources[source]->play(sound);
        }

        SoundBuffer *soundBuffer = nullptr;

    private:
        SoundDevice *soundDevice = nullptr;

        unsigned int nSources = 5;
        std::vector<SoundSource *> sources = {};
    };
}
