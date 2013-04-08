//Plays the shoot sound whenever BUT (onboard button) is pressed. 
//Requires a speaker connected to P3 and GND.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "speaker.h"

#include <libmaple/gpio.h>
#include <libmaple/exti.h>

void pushButton(void){
    speaker_playShoot();
}

// setup() and loop():
void setup(void) {
    speaker_start();

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
