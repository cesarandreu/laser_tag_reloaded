//Plays the shoot sound whenever BUT (onboard button) is pressed. 
//Requires a speaker connected to P3 and GND.

//Local includes:
#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/exti.h>
#include "initialize.h"
#include "speaker.h"
#include "receiver.h"
#include "sender.h"
#include "trigger.h"
#include "delay.h"

void pushButton(void){
    sender_shoot();
    speaker_playShoot();
    trigger_delay_start();
}

void test(void){
    speaker_playHit();
    delay_start();
}

// setup() and loop():
void setup(void) {
    speaker_start();
    sender_start(8);
    receiver_start();
    delay_init();

    receiver_setInterrupt(test);
    //Onboard button
    trigger_start();
    trigger_set_interrupt(pushButton);

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
