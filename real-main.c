

//Local includes:
#include "initialize.h"
#include "usb.h"

// setup() and loop():
void setup(void) {
    usb_start();

}

void loop(void) {
    usb_printlnString("This is a banana!");
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
