//Plays the shoot sound whenever BUT (onboard button) is pressed.
//Requires a speaker connected to P3 and GND.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "communication.h"
#include <libmaple/gpio.h>
#include <libmaple/exti.h>
#include "storage.h"



void transmit_interrupt(void){
    hit result = storage_getShot();
    if(result.ID!=0){
        transmit_hitData(result);
    }
}

// setup() and loop():
void setup(void) {
    
    storage_start();
    storage_add(16, "SOME LOCATION");
    storage_add(16, "ANOTHER LOCATION");


    //bluetooth_start();
    communication_start();

    gpio_set_mode(GPIOB, 8, GPIO_INPUT_PD);
    exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, transmit_interrupt, EXTI_RISING);

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
