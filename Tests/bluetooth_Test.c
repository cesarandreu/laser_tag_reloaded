//Bluetooth test.
//Requires the bluetooth module be connected at pins 8 and 9.
//If the receivedChar is 'a' then it turns on the onboard LED.
//Also prints to the usb_serial whatever value it has. 


//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "usb.h"
#include "bluetooth.h"
#include <libmaple/gpio.h>

//Bluetooth test
char currentChar = 'a';

// setup() and loop():
void setup(void) {
    usb_start();
    bluetooth_start();


    //Onboard LED
    gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOB, 1, 1);

}

void loop(void) {
    bluetooth_writeChar(currentChar);

    if(bluetooth_available()){
        currentChar = bluetooth_read();
        usb_printString("Received value is: ");
        usb_printlnChar(currentChar);
    } else {
        usb_printString("Current value is: ");
        usb_printlnChar(currentChar);
    }

    if(currentChar=='a'){
        gpio_write_bit(GPIOB, 1, 1);
    } else {
        gpio_write_bit(GPIOB, 1, 0);
    }

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
