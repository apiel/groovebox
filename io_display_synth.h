#ifndef IO_DISPLAY_SYNTH_H_
#define IO_DISPLAY_SYNTH_H_

#include <Adafruit_SSD1306.h>

#include "io_display.h"
#include "io_display_util.h"

const char* getWave(byte w) {
    if (w < WAVEFORM_COUNT) {
        switch (w) {
            case 0:
                return "Sine";
            case 1:
                return "Sawtooth";
            case 2:
                return "Square";
            case 3:
                return "Triangle";
            case 4:
                return "Arbitrary";
            case 5:
                return "Pulse";
            case 6:
                return "Saw reverse";
            case 7:
                return "Sample hold";
            case 8:
                return "Tri var";
        }
    }
    IO_AudioSynthWaveListRaw* r = r->getInstance();
    w = w - WAVEFORM_COUNT;
    if (w < r->rawWaveCount) {
        return r->wavetableName[w];
    }
    return "unknown";
}

const char* getFilter(byte w) {
    switch (w) {
        case 0:
            return "LowPass";
        case 1:
            return "BandPass";
        case 2:
            return "HighPass";
    }
    return "unknown";
}

void displaySynth(Adafruit_SSD1306* d) {
    d->clearDisplay();
    d->setCursor(0, 0);

    d->printf("%d| %s\n", currentSynth,
              getWave(synth[currentSynth].wave.currentWaveform));

    d->printf("%.1fHz %d%%\n", synth[currentSynth].wave.frequency,
              (int)(synth[currentSynth].wave.amplitude * 100));

    if (synth[currentSynth].useAdsr) {
        d->printf("ADSR %d|%d|%d%%|%d\n", (int)synth[currentSynth].attackMs,
                  (int)synth[currentSynth].decayMs,
                  (int)(synth[currentSynth].sustainLevel * 100),
                  (int)synth[currentSynth].releaseMs);
    } else {
        d->println("ADSR off");
    }

    addToCursor(d, 0, 4);
    d->printf("%s %.1fHz\n", getFilter(synth[currentSynth].currentFilter),
              synth[currentSynth].filterFrequency);
    d->printf("Reso %.1f Octave %.1f\n", synth[currentSynth].filterResonance,
              synth[currentSynth].filterOctaveControl);

    addToCursor(d, 0, 4);
    if (synth[currentSynth].modulation.currentModulation == MOD_ENV) {
        d->printf("mod %d|%d|%d%%|%d\n",
                  (int)synth[currentSynth].modulation.modAttackMs,
                  (int)synth[currentSynth].modulation.modDecayMs,
                  (int)(synth[currentSynth].modulation.modSustainLevel * 100),
                  (int)synth[currentSynth].modulation.modReleaseMs);
    } else if (synth[currentSynth].modulation.currentModulation == MOD_LFO) {
        d->printf("Lfo %.1fHz %d%% %s\n",
                  synth[currentSynth].modulation.lfoFrequency,
                  (int)(synth[currentSynth].modulation.lfoAmplitude * 100),
                  getWave(synth[currentSynth].modulation.lfoWave));
    }
}

#endif
