# synth

- modulation
    - create a random drop modulation
    - on each phase could change freq start...

- save synth settings
    - in the 6 default spot
    - but give the possibility to save in extra spot or load from extra spot
- save synth into a raw wavetable

- arbitrary wave, load from file

- think about a 3 voice synth, with a dedicated osc per voice

# keyboard

- Should there be a master keyboard and then a keyboard that can be use for anything???

- select input and output
    - input is midi channel of keyboard
    - output can be midi channel or internal instrument: synth, sample, ...
- save setting in a file

# midi

- send midi clock

# chaining

# sequencer

- send gate to gpio
- save sequencer
- multiple sequencer switch
- load sequencer

# sequence live edit

# Pattern

- edit slide
- edit name
- save
    - should edit be directly reflected
        - should we save as soon we change something
        - there could be an auto save? and in that case it is reflected...
    - when we change of pattern, should it ask for save
- assign pattern to output

# samples

- dont use wav sample, too much trouble

# Links

- Multiple DAC & ADC
    - https://forum.pjrc.com/threads/49232-6-inputs-audio-recorder-with-CS42448-audio-card-from-OSH-Park
    - https://www.aliexpress.com/wholesale?catId=0&initiative_id=SB_20210311222304&SearchText=CS42448
    - https://hackaday.io/project/2984-teensy-audio-library/log/57537-tdm-support-for-many-channel-audio-io

    - ? https://forum.pjrc.com/threads/27215-24-bit-audio-boards?highlight=eurorack+teensy
    - https://github.com/mxmxmx/O_C
    - +++ https://little-scale.blogspot.com/2017/11/usb-midi-to-eight-gates-and-sixteen-cv.html?m=1
    - +++ https://hackaday.io/project/99863-mad-11-modular-audio-devices

- Wave table from SD
    - https://forum.pjrc.com/threads/58735-Wavetable-Synthesis-From-SD-Card
    - https://forum.pjrc.com/threads/62587-WaveTable-audio-Datas-and-structures-on-SD

- Midi controller
    - http://little-scale.blogspot.com/2018/12/teensy-36-as-standalone-mediator.html
