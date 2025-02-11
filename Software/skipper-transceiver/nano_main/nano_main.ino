#include "../nano.h"

Nano skipperNano;

void setup() {
    skipperNano.init();
}

void loop() {
    skipperNano.receiveCommand();
    skipperNano.checkConnection();
    delay(500);
}
