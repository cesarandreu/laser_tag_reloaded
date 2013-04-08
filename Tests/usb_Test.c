//This test prints 'This is a test!' to the serial. 
//Simply connect to the USB's serial from a computer.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "usb.h"


// setup() and loop():
void setup(void) {
    usb_start();

}

void loop(void) {
    usb_printlnString("This is a test!");

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
