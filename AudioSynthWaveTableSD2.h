#ifndef AUDIO_SYNTH_WAVE_TABLE_SD_H_
#define AUDIO_SYNTH_WAVE_TABLE_SD_H_

#include <Arduino.h>
#include <AudioStream.h>

#include "note.h"

template <uint16_t MAX_TABLE_SIZE = 24000>
class AudioSynthWaveTableSD : public AudioStream {
   public:
    AudioSynthWaveTableSD()
        : AudioStream(1, inputQueueArray), magnitude(16384) {}
    AudioSynthWaveTableSD *frequency(float freq) {
        if (freq < 0.0f) {
            freq = 0.0;
        } else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2.0f) {
            freq = AUDIO_SAMPLE_RATE_EXACT / 2.0f;
        }
        phase_increment = freq * (4294967296.0f / AUDIO_SAMPLE_RATE_EXACT);
        if (phase_increment > 0x7FFE0000u) phase_increment = 0x7FFE0000;
        return this;
    }

    AudioSynthWaveTableSD *amplitude(float n) {
        if (n < 0) {
            n = 0;
        } else if (n > 1.0f) {
            n = 1.0;
        }
        magnitude = n * 65536.0f;
        return this;
    }

    void update(void) {
        audio_block_t *block;
        int16_t *bp, *end2;
        int32_t val1, val2;
        uint32_t i, ph, index, index2, scale;
        const uint32_t inc = phase_increment;

        int16_t tone_offset = 0;
        uint32_t phase_offset = 0;

        ph = phase_accumulator + phase_offset;
        if (magnitude == 0) {
            phase_accumulator += inc * AUDIO_BLOCK_SAMPLES;
            return;
        }
        block = allocate();
        if (!block) {
            phase_accumulator += inc * AUDIO_BLOCK_SAMPLES;
            return;
        }
        bp = block->data;

        int16_t *arbdata = data;
        if (!arbdata) {
            // Serial.println("exit not arbdata");
            release(block);
            phase_accumulator += inc * AUDIO_BLOCK_SAMPLES;
            return;
        }

        // Serial.printf("%d of %d\n", ph, end);
        for (i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            index = ph >> 24;
            index2 = index + 1;
            if (index2 >= end) index2 = 0;
            val1 = *(arbdata + index);
            val2 = *(arbdata + index2);
            scale = (ph >> 8) & 0xFFFF;
            val2 *= scale;
            val1 *= 0x10000 - scale;
            *bp++ = multiply_32x32_rshift32(val1 + val2, magnitude);
            ph += inc;
        }

        phase_accumulator = ph - phase_offset;

        if (tone_offset) {
            bp = block->data;
            end2 = bp + AUDIO_BLOCK_SAMPLES;
            do {
                val1 = *bp;
                *bp++ = signed_saturate_rshift(val1 + tone_offset, 16, 0);
            } while (bp < end2);
        }
        transmit(block, 0);
        release(block);
    }

    bool load(const char *filename) {
        File rawfile;
        rawfile = SD.open(filename);
        if (!rawfile || !rawfile.available()) {
            return false;
        }
        size = rawfile.read(data, min(MAX_TABLE_SIZE, rawfile.size()));
        end = min(end, size);

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
        end = min(val, size ? size : MAX_TABLE_SIZE);
        return this;
    }

    AudioSynthWaveTableSD *reset() {
        phase_accumulator = 0;
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

    audio_block_t *inputQueueArray[1];
};

#endif
