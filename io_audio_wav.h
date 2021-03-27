#ifndef IO_AUDIO_WAV_H_
#define IO_AUDIO_WAV_H_

#include <Arduino.h>
#include <Audio.h>

#include "note.h"

#define WAV_NAME_LEN 20
#define WAV_COUNT 12

class IO_AudioWav : public AudioPlaySdWav {
   public:
    byte folder = 0;

    bool play(byte note) {
        char idx = note - _C4;
        char filename[40];
        sprintf(filename, "wav/%d/%02d.wav", folder, idx);
        Serial.println(filename);

        AudioPlaySdWav::play(filename);
    }

    void setFolder(byte _folder) {
        folder = _folder;
    }
};

#endif
