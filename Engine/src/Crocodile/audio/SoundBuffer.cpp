#include "SoundBuffer.h"

namespace Crocodile
{
    SoundBuffer *SoundBuffer::get()
    {
        static SoundBuffer *snd_buffer = new SoundBuffer();
        return snd_buffer;
    }

    SoundBuffer::SoundBuffer()
    {
    }

    SoundBuffer::~SoundBuffer()
    {
        soundEffects.clear();
    }

#ifndef CROCODILE_EMSCRIPTEN
    void SoundBuffer::addSoundEffect(const char *filename, std::string name)
    {
        ALenum err, format;
        ALuint buffer;
        SNDFILE *sndfile;
        SF_INFO sfinfo;
        short *membuf;
        sf_count_t num_frames;
        ALsizei num_bytes;

        /* Open the audio file and check that it's usable. */
        sndfile = sf_open(filename, SFM_READ, &sfinfo);
        if (!sndfile)
        {
            fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
            return;
        }
        if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
        {
            sf_close(sndfile);
            return;
        }

        /* Get the sound format, and figure out the OpenAL format */
        format = AL_NONE;
        if (sfinfo.channels == 1)
            format = AL_FORMAT_MONO16;
        else if (sfinfo.channels == 2)
            format = AL_FORMAT_STEREO16;
        else if (sfinfo.channels == 3)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT2D_16;
        }
        else if (sfinfo.channels == 4)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT3D_16;
        }
        if (!format)
        {
            fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
            sf_close(sndfile);
            return;
        }

        /* Decode the whole audio file to a buffer. */
        membuf = static_cast<short *>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
        if (num_frames < 1)
        {
            free(membuf);
            sf_close(sndfile);
            return;
        }
        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

        /* Buffer the audio data into a new buffer object, then free the data and
         * close the file.
         */
        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

        free(membuf);
        sf_close(sndfile);

        /* Check if an error occured, and clean up if so. */
        err = alGetError();
        if (err != AL_NO_ERROR)
        {
            fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
            if (buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return;
        }

        soundEffects[name] = buffer;
    }
#else
    void SoundBuffer::addSoundEffect(const char *filename, std::string name)
    {
    }
#endif

    void SoundBuffer::removeSoundEffect(std::string name)
    {
        auto it = soundEffects.find(name);
        if (it != soundEffects.end())
        {
            soundEffects.erase(it);
        }
    }

    ALuint SoundBuffer::getSound(std::string name)
    {
        return soundEffects[name];
    }

}