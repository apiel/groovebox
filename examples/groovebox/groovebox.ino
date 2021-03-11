#include <groovebox.h>

void setup() {
    Serial.begin(115200);
    grooveboxInit();
}

void loop() {
    grooveboxLoop();
}
