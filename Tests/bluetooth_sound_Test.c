//Plays the shoot sound whenever BUT (onboard button) is pressed. 
//Requires a speaker connected to P3 and GND.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "speaker.h"
#include "bluetooth.h"

#include <libmaple/gpio.h>
#include <libmaple/exti.h>

char currentChar = 'a';

void pushButton(void){
    speaker_playHit();
}

// setup() and loop():
void setup(void) {
    bluetooth_start();
    speaker_start();

    //Onboard button
    gpio_set_mode(GPIOB, 8, GPIO_INPUT_PU);
    exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, pushButton, EXTI_FALLING);

    //Onboard LED
    gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOB, 1, 1);
}

void loop(void) {

    //bluetooth_writeChar(currentChar);

    if(bluetooth_available()){
        currentChar = bluetooth_read();
        bluetooth_writeChar(currentChar);
        if(currentChar=='s'){
            speaker_playShoot();
        }
        if(currentChar=='h'){
            speaker_playHit();
        }
        if(currentChar=='a'){
            gpio_write_bit(GPIOB, 1, 1);
        } else {
            gpio_write_bit(GPIOB, 1, 0);
        }
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
