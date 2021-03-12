#include <io_groovebox.h>

void setup() {
    Serial.begin(115200);
    ioGrooveboxInit();
}

void loop() {
    ioGrooveboxLoop();
}
