//Plays the shoot sound whenever BUT (onboard button) is pressed. 
//Requires a speaker connected to P3 and GND.
//Shoots the player code (12) through the IR LED at D13.

//Local includes:
#include "initialize.h"
#include "speaker.h"
#include "sender.h"

#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/exti.h>



void pushButton(void){
    sender_shoot();
    speaker_playShoot();
}


// setup() and loop():
void setup(void) {

    speaker_start();
    sender_start();
    sender_setPlayerCode(12);

    //Onboard button
    gpio_set_mode(GPIOB, 8, GPIO_INPUT_PU);
    exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, pushButton, EXTI_FALLING);
    

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
