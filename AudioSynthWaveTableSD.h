#ifndef AUDIO_SYNTH_WAVE_TABLE_SD_H_
#define AUDIO_SYNTH_WAVE_TABLE_SD_H_

#include <Arduino.h>
#include <AudioStream.h>

#include "note.h"

// AUDIO_SAMPLE_RATE_EXACT 44100

template <uint16_t MAX_TABLE_SIZE = 24000>
class AudioSynthWaveTableSD : public AudioStream {
   public:
    AudioSynthWaveTableSD() : AudioStream(0, NULL), magnitude(16384) {}
    AudioSynthWaveTableSD *frequency(float freq) {
        if (freq <= 0.0) {
            phase_increment = 0.0;
        } else {
            phase_increment = freq / baseFreq;
        }
        return this;
    }

    AudioSynthWaveTableSD *amplitude(float n) {
        magnitude = constrain(n, 0.0, 1.0);
        return this;
    }

    void update(void) {
        audio_block_t *block = allocate();

        if (block == NULL || magnitude == 0.0 || phase_increment == 0.0)
            return;  // but here we might still want to increase the
                     // phase_accumulator

        for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            phase_accumulator += phase_increment;
            if ((uint32_t)phase_accumulator > end) {
                phase_accumulator = start;
            }
            block->data[i] =
                (int16_t)(data[(uint32_t)phase_accumulator] * magnitude);
        }

        transmit(block);
        release(block);
    }

    bool load(const char *filename) {
        File rawfile;
        rawfile = SD.open(filename);
        if (!rawfile || !rawfile.available()) {
            return false;
        }
        size = rawfile.read(data, min(MAX_TABLE_SIZE, rawfile.size()));
        end = size;
        // end = min(end, size);

        rawfile.close();
        return true;
    }

    AudioSynthWaveTableSD *setBaseFreq(float freq) {
        baseFreq = freq;
        return this;
    }

    AudioSynthWaveTableSD *setStart(uint32_t val) {
        start = val;
        return this;
    }

    AudioSynthWaveTableSD *setEnd(uint32_t val) {
        end = min(val, size);
        return this;
    }

   private:
    float baseFreq = NOTE_FREQ[_C4];
    float phase_accumulator;
    float phase_increment;
    float magnitude;
    int16_t data[MAX_TABLE_SIZE];

    uint32_t size = 0;
    uint32_t end = MAX_TABLE_SIZE;
    uint32_t start = 0;
};

#endif
