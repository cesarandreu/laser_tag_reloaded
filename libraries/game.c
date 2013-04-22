
#include "game.h"
#include "communication.h"
#include "enemy.h"
#include "GPS.h"
#include "player.h"
#include "receiver.h"
#include "sender.h"
#include "storage.h"


//Testing
//--{
//#include <libmaple/gpio.h>
//#include <libmaple/exti.h>
//
//void game_triggerButton(void){
//    sender_shoot();
//    player_shoot();
//    speaker_playShoot();
//}
//--}

void game_receiverInterrupt(void){
    int receivedCode = receiver_listenSignal();

    //Should use GPS_COORDINATE_LENGTH
    char gps_location[25] = "SOME GPS LOCATION... 123";

    if(receivedCode>0 && receivedCode<=256){

        if(enemy_checkExist(receivedCode)!=0){

        //Some function to turn on GPS and the location data.

        storage_add(receivedCode, gps_location);
        transmit_hitData(storage_getShot());
        transmit_playerData(player_getShots());
        speaker_playHit();

        //Some function that sets up a timer and disables the received and shooter.
        //Then it waits like 5~ seconds.
        //After the 5 seconds are up, it enables the received again and shooter. 
        }

    } else {
        hit toSend = storage_getShot();
        if(toSend.ID!=0){
            transmit_hitData(toSend);
        }

    }

}

void game_new(void){
    enemy_start();
    //gps_start();
    player_start(DEFAULT_PLAYER_CODE);
    sender_start(DEFAULT_PLAYER_CODE);
    speaker_start();
    receiver_setInterrupt(game_receiverInterrupt);
    storage_start();

    //Some function to set the trigger button as an input, but not enable its interrupt.

}

void game_start(void){
    //gps_end();
    receiver_enable();

    //Some function to set the trigger button's interrupt.

    //Testing, remove later.
    //--{
    //gpio_set_mode(GPIOA, 13, GPIO_INPUT_PD);
    //exti_attach_interrupt(AFIO_EXTI_13, AFIO_EXTI_PA, game_triggerButton, EXTI_RISING);
    //--}

}

void game_information(char gameType, int playerNumber, int gameLimit, int enemyNumber, int enemyList[]){
    
    int i=0;
    for(i=0; i<enemyNumber; i++){
        enemy_addEnemy(enemyList[i]);
    }
    
    player_setCode(playerNumber);
    sender_setPlayerCode(playerNumber);

}

void game_end(int statusCode){

    //Some function to disable the trigger button's interrupt.
    
    //Testing, remove later.
    //--{
    //exti_detach_interrupt(AFIO_EXTI_13);
    //--}

    receiver_disable();
    gps_end();
    transmit_playerData(player_getShots());
    player_reset();
    enemy_reset();

    //Get all the remaining data and transmit it. 
    int i=0;
    for(i=0; i<storage_unsentEntries(); i++){
        hit toSend = storage_getShot();
        transmit_hitData(toSend);
        storage_removeHit(toSend.hitNumber);
    }

    storage_reset();


    /* TO-DO Later: Add win/lose sound.
    if(statusCode==1){
        speaker_playWin();
    } else{
        speaker_playLose();
    }
    */

}

