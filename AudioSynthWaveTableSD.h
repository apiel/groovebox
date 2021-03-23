#ifndef AUDIO_SYNTH_WAVE_TABLE_SD_H_
#define AUDIO_SYNTH_WAVE_TABLE_SD_H_

#include <Arduino.h>
#include <AudioStream.h>

// AUDIO_SAMPLE_RATE_EXACT 44100

template <uint16_t MAX_TABLE_SIZE = 24000>
class AudioSynthWaveTableSD : public AudioStream {
   public:
    AudioSynthWaveTableSD() : AudioStream(0, NULL), magnitude(16384) {}
    void frequency(float freq) {
        if (freq < 0.0) {
            freq = 0.0;
        } else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2) {
            freq = AUDIO_SAMPLE_RATE_EXACT / 2;
        }
        phase_increment = freq * (4294967296.0 / AUDIO_SAMPLE_RATE_EXACT); // 4294967296 = 2³²
        if (phase_increment > 0x7FFE0000u) phase_increment = 0x7FFE0000;
    }

    void phase(float angle) {
        if (angle < 0.0) {
            angle = 0.0;
        } else if (angle > 360.0) {
            angle = angle - 360.0;
            if (angle >= 360.0) return;
        }
        phase_offset = angle * (4294967296.0 / 360.0);
    }

    void amplitude(float n) {  // 0 to 1.0
        if (n < 0) {
            n = 0;
        } else if (n > 1.0) {
            n = 1.0;
        }
        magnitude = n;
    }

    void update(void) {
        audio_block_t *block = allocate();

        if (block == NULL) return;

        if (magnitude == 0.0)
            return;  // but here we might still want to increase the
                     // phase_accumulator

        for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            phase_accumulator++;
            if (phase_accumulator > size) {
                phase_accumulator = 0;
            }
            block->data[i] = (int16_t)(data[phase_accumulator] * magnitude);
        }

        transmit(block);
        release(block);
    }

    // void update(void) {
    //     audio_block_t *block;
    //     int16_t *bp, *end;
    //     int32_t val1, val2;
    //     int16_t magnitude15;
    //     uint32_t i, ph, index, index2, scale;
    //     const uint32_t inc = phase_increment;

    //     if (!size) {
    //         return;
    //     }

    //     ph = phase_accumulator + phase_offset;
    //     if (magnitude == 0) {
    //         phase_accumulator += inc * AUDIO_BLOCK_SAMPLES;
    //         return;
    //     }
    //     block = allocate();
    //     if (!block) {
    //         phase_accumulator += inc * AUDIO_BLOCK_SAMPLES;
    //         return;
    //     }
    //     bp = block->data;

    //     for (i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    //         index = ph >> 24;
    //         index2 = index + 1;
    //         if (index2 >= size) index2 = 0;
    //         val1 = data[index];
    //         val2 = data[index2];
    //         scale = (ph >> 8) & 0xFFFF;
    //         val2 *= scale;
    //         val1 *= 0x10000 - scale;
    //         *bp++ = multiply_32x32_rshift32(val1 + val2, magnitude);
    //         ph += inc;
    //     }

    //     phase_accumulator = ph - phase_offset;

    //     transmit(block, 0);
    //     release(block);
    // }

    bool load(const char *filename) {
        File rawfile;
        rawfile = SD.open(filename);
        if (!rawfile) {
            return false;
        }
        uint32_t file_offset = 0;

        if (!rawfile.available()) {
            return false;
        }
        size = rawfile.read(data, min(MAX_TABLE_SIZE, rawfile.size()));

        Serial.print("load file ");
        Serial.println(size);
        Serial.println(phase_increment);

        // for (int i = 0; i < size; i++) {
        //     Serial.println(data[i]);
        // }
        // Serial.println("end");

        rawfile.close();
        return true;
    }

   private:
    uint32_t phase_offset;
    uint32_t phase_accumulator;
    uint32_t phase_increment;
    float magnitude;
    int16_t data[MAX_TABLE_SIZE];

    uint32_t size;
};

#endif
