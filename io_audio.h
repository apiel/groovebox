#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>
#include <Metro.h>

#include "io_audio_synth.h"

class IO_Audio {
   public:
    AudioOutputMQS audioOut;
    // IO_AudioSynth &synth;
    IO_AudioSynth synth;
    // Metro timerHold = Metro(150);
    // Metro timerNote = Metro(1000);
    // bool io_playing = true;

    IO_Audio() : synth(&audioOut) { AudioMemory(10); }
    // IO_Audio() : synth(10) { AudioMemory(10); }
    // IO_Audio() { AudioMemory(10); }

    void loop() {
        // if (io_playing && timerNote.check() == 1) {
        //     timerNote.reset();
        //     timerHold.reset();
        //     synth.noteOn();
        // }
        // if (timerHold.check() == 1) {
        //     synth.noteOff();
        // }
    }

    // void audioPlay(bool playing) { io_playing = playing; }
    // bool audioIsPlaying() { return io_playing; }
};

#endif
