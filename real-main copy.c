//Plays the shoot sound whenever BUT (onboard button) is pressed.
//Requires a speaker connected to P3 and GND.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include "communication.h"
#include <libmaple/gpio.h>
#include <libmaple/exti.h>
#include <string.h>
#include "storage.h"
#include "speaker.h"
#include "receiver.h"
#include "sender.h"
#include "trigger.h"
#include "gps.h"
#include "usb.h"

int gps_check = 0;
char str[25] = "No Coordinates Available";


void transmit_interrupt(void){
    hit result = storage_getShot();
    if(result.ID!=0){
        transmit_hitData(result);
    }
}

void pushButton(void){
    sender_shoot();
    speaker_playShoot();
    gps_check = 1;
    //char str[25] = "No Coordinates Available";
    //gps_getLocation(str, 25);
    //usb_printlnString(str);
}

// setup() and loop():
void setup(void) {

    batteryMonitor_initialize();
    
    usb_start();
    gps_start();

    //storage_start();
    //storage_add(16, "SOME LOCATION");
    //storage_add(16, "ANOTHER LOCATION");

    //bluetooth_start();
    communication_start();

    speaker_start();
    sender_start(8);
    receiver_start();

    //Onboard button
    trigger_start();
    trigger_set_interrupt(pushButton);

    //gpio_set_mode(GPIOB, 8, GPIO_INPUT_PD);
    //exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, transmit_interrupt, EXTI_RISING);

    //Onboard LED
    gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOB, 1, 1);
}

void loop(void) {
    gpio_write_bit(GPIOB, 1, gps_check);
    if(gps_check == 1)
    {
        //gps_getLocation(str, 25);
        usb_printlnString(str);
        gps_check = 0;
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
