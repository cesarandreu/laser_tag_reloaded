//Plays the shoot sound whenever BUT (onboard button) is pressed.
//Requires a speaker connected to P3 and GND.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "communication.h"
#include "usb.h"
#include "ledGraph.h"

#include <libmaple/gpio.h>

#include "player.h"

#include "speaker.h"
#include "trigger.h"

/*void transmit_interrupt(void){
    hit result = storage_getShot();
    if(result.ID!=0){
        transmit_hitData(result);
    }
}*/

/*
void toggleLED(void){
    gpio_toggle_bit(GPIOB, 1);
}
*/



void laserTest(void){
    sender_shoot();
    speaker_playShoot();
    gpio_toggle_bit(GPIOB, 1);

}



// setup() and loop():
void setup(void) {
    
/*    storage_start();
    storage_add(16, "SOME LOCATION");
    storage_add(16, "ANOTHER LOCATION");
*/

    //usb_end();
    //batteryMonitor_initialize();
    //communication_start();


    
    speaker_start();

    //gpio_set_mode(GPIOB, 8, GPIO_INPUT_PD);
    //exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, speakerTest, EXTI_RISING);

    //exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, fakeShot, EXTI_RISING);

    //Onboard LED
    gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOB, 1, 1);
    

    sender_start(10);
    trigger_set_interrupt(laserTest);
    trigger_start();




    //Battery gauge code magic goes here. 
    //Initialize the battery gauge and its trigger.


    //LED
    //gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    //gpio_write_bit(GPIOB, 1, 1);

    //Onboard Button
    //gpio_set_mode(GPIOB, 8, GPIO_INPUT_PD);
    //exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, fakeShot, EXTI_RISING);
    
    

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
