//GPS Test
//This test prints data from the GPS to the USB port.
//Connect to the USB's serial from a computer and connect the GPS Pin 1 to Pin 26 and GPS Pin 2 to Pin 25

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "usb.h"
#include "GPS.h"


// setup() and loop():
void setup(void) {
    usb_start();
    gps_start();
}

void loop(void) {
    if(gps_available())
        usb.writeChar(gps_read());
    if(usb_available())
        gps_write(usb_read());
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
