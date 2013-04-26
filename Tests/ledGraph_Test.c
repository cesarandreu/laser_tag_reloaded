#include "ledGraph.h"

void setup(void) {
    batteryMonitor_initialize();
}

void loop(void) {
}


__attribute__((constructor)) void premain() {
    init();
}

int main(void) {
    setup();

    while (1) {
        loop();
    }

    return 0;
}