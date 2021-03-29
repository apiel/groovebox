#ifndef IO_AUDIO_SYNTH_STORAGE_H_
#define IO_AUDIO_SYNTH_STORAGE_H_

#include <SD.h>

#include "io_state.h"
#include "io_storage.h"

void setSynthPath(byte pos) {
    setFilePath("synth/%03d.io", pos);
}

bool saveSynth(byte synthPos) {
    Serial.println("saveSynth");
    if (sdAvailable) {
        setSynthPath(synthPos);
        File file = SD.open(filePath, FILE_WRITE);

        if (file) {
            file.seek(0);
            // sprintf(storageBuffer, "%s\n", patterns[patternPos].name);
            // file.print(storageBuffer);

            file.close();
            return true;
        }
    }
    Serial.println("- failed to open file for writing");
    return false;
}

#endif
