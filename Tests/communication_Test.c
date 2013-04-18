//Can parse received data. This is a test for communication.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "communication.h"
#include <libmaple/gpio.h>
#include <libmaple/exti.h>


// setup() and loop():
void setup(void) {
    

    //bluetooth_start();
    communication_start();


    //Onboard LED
    gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOB, 1, 1);
    



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
