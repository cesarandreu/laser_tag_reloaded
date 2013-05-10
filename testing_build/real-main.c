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


void fakeShot(void){

    if (player_getCode()==8){

    gpio_toggle_bit(GPIOB, 1);

    int receivedCode = 10;

    char gps_location[25] = "GPS_DATA_NOT_VALID_SORRY";

    if(receivedCode>0 && receivedCode<=256){

        if(enemy_checkExist(receivedCode)!=0){

        //Some function to turn on GPS and the location data.
        //gps_getLocation(gps_location);
        storage_add(receivedCode, gps_location);
        transmit_hitData(storage_getShot());
        transmit_playerData(player_getShots());
        return;
        //speaker_playHit();

        //Some function that sets up a timer and disables the received and shooter.
        //Then it waits like 5~ seconds.
        //After the 5 seconds are up, it enables the received again and shooter. 
        }

    } 


    } else {

    gpio_toggle_bit(GPIOB, 1);

    int receivedCode = 8;

    char gps_location[25] = "GPS_DATA_NOT_VALID_SORRY";

    if(receivedCode>0 && receivedCode<=256){

        if(enemy_checkExist(receivedCode)!=0){

        //Some function to turn on GPS and the location data.
        //gps_getLocation(gps_location);
        storage_add(receivedCode, gps_location);
        transmit_hitData(storage_getShot());
        transmit_playerData(player_getShots());
        return;
        //speaker_playHit();

        //Some function that sets up a timer and disables the received and shooter.
        //Then it waits like 5~ seconds.
        //After the 5 seconds are up, it enables the received again and shooter. 
        }

    } 

    }


}

void laserTest(void){
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


    
    //speaker_start();

    //gpio_set_mode(GPIOB, 8, GPIO_INPUT_PD);
    //exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, speakerTest, EXTI_RISING);

    //exti_attach_interrupt(AFIO_EXTI_8, AFIO_EXTI_PB, fakeShot, EXTI_RISING);

    trigger_set_interrupt(laserTest);
    trigger_start();

    //Onboard LED

    gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOB, 1, 1);
    


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
