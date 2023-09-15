
#pragma once
#include <AL/al.h>
#ifndef CROCODILE_EMSCRIPTEN
#include <sndfile.h>
#include <AL/alext.h>
#endif
#include <inttypes.h>
#include <vector>
#include <map>
#include <limits.h>
#include <stdlib.h>

#include <iostream>
#include <type_traits>
#include <cstring>
#include <fstream>

#include "../Core.h"

namespace Crocodile
{
    class CROCODILE_API SoundBuffer
    {
    public:
        static SoundBuffer *get();

#ifndef CROCODILE_EMSCRIPTEN
        void addSoundEffect(const char *filename, std::string name);
#endif
        void removeSoundEffect(std::string name);

        ALuint getSound(std::string name);
        bool isSoundPlaying(std::string name);

    private:
        SoundBuffer();
        ~SoundBuffer();

        std::map<std::string, ALuint> soundEffects = {};
    };
}