//GPS Test
//This test prints data from the GPS to the USB port.
//Connect to the USB's serial from a computer and connect the GPS Pin 1 to Pin 26 and GPS Pin 2 to Pin 25

//Local includes:
#include <libmaple/libmaple.h>
#include <libmaple/delay.h>
#include "initialize.h"
#include "usb.h"
#include "GPS.h"
#include <string.h>


// setup() and loop():
void setup(void) {
    usb_start();
    gps_start();
    delay_us(1000);
}

void loop(void) {
    /*while(gps_hasFix() == 0)
        usb_printlnString("No Coordinates Available");*/
    char str[25] = "No Coordinates Available";
    gps_getLocation(str, 25);
    usb_printlnString(str);
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
