#ifndef IO_DISPLAY_SYNTH_H_
#define IO_DISPLAY_SYNTH_H_

#include <Adafruit_SSD1306.h>

#include "io_display.h"
#include "io_display_util.h"

const char* getWave(byte w) {
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
              getWave(synth[currentSynth].currentWaveform));

    d->printf("%.1fHz %d%%\n", synth[currentSynth].frequency,
              (int)(synth[currentSynth].amplitude * 100));

    d->printf("ADSR %d|%d|%d%%|%d\n", (int)synth[currentSynth].attackMs,
              (int)synth[currentSynth].decayMs,
              (int)(synth[currentSynth].sustainLevel * 100),
              (int)synth[currentSynth].releaseMs);

    addToCursor(d, 0, 4);
    d->printf("%s %.1fHz\n", getFilter(synth[currentSynth].currentFilter),
              synth[currentSynth].filterFrequency);
    d->printf("Reso %.1f Octave %.1f\n", synth[currentSynth].filterResonance,
              synth[currentSynth].filterOctaveControl);

    addToCursor(d, 0, 4);
    d->printf("mod %d|%d|%d%%|%d\n", (int)synth[currentSynth].modAttackMs,
              (int)synth[currentSynth].modDecayMs,
              (int)(synth[currentSynth].modSustainLevel * 100),
              (int)synth[currentSynth].modReleaseMs);
    d->printf("Lfo %.1fHz %d%% %s\n", synth[currentSynth].lfoFrequency,
              (int)(synth[currentSynth].lfoAmplitude * 100),
              getWave(synth[currentSynth].lfoWave));
}

#endif
