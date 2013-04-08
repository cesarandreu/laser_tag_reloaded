//Plays the shoot sound every 5 seconds. 
//Requires a speaker connected to P3 and GND.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "speaker.h"


// setup() and loop():
void setup(void) {
    speaker_start();
}

void loop(void) {
    speaker_playShoot();
    delay_us(5000000);
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
