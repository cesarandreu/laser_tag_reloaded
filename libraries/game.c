#include "game.h"
//#include "communication.h"
//#include "enemy.h"
//#include "GPS.h"
//#include "player.h"
//#include "receiver.h"
//#include "receiverBack.h"
//#include "receivedCommon.h"
//#include "sender.h"
//#include "speaker.h"
//#include "storage.h"
//#include "trigger.h"

//Testing
//--{
/*
#include <libmaple/gpio.h>
#include <libmaple/exti.h>

void game_triggerButton(void){
    sender_shoot();
    player_shoot();
    speaker_playShoot();
}
*/
//--}

void game_triggerInterrupt(void){
    sender_shoot();
    player_shoot();
    speaker_playShoot();
    trigger_delay_start();
}

void game_receiverInterruptA(void){
    nvic_globalirq_disable();

    int receivedCode = receiver_listenSignal();

    //Should use GPS_COORDINATE_LENGTH
    char gps_location[25] = "GPS_DATA_NOT_VALID_SORRY";

    if(receivedCode>0 && receivedCode<=256){

        if(enemy_checkExist(receivedCode)!=0){

            gps_getLocation(gps_location, 25);            
            transmit_hitData(storage_add(receivedCode, gps_location));
            transmit_playerData(player_getShots());
            speaker_playHit();

        //Some function that sets up a timer and disables the received and shooter.
        //Then it waits like 5~ seconds.
        //After the 5 seconds are up, it enables the received again and shooter. 
        }

    } 

    delay_start();
    nvic_globalirq_enable();
}

void game_receiverInterruptB(void){
    nvic_globalirq_disable();

    int receivedCode = receiverB_listenSignal();

    //Should use GPS_COORDINATE_LENGTH
    char gps_location[25] = "GPS_DATA_NOT_VALID_SORRY";

    if(receivedCode>0 && receivedCode<=256){

        if(enemy_checkExist(receivedCode)!=0){

            gps_getLocation(gps_location, 25);
            transmit_hitData(storage_add(receivedCode, gps_location));
            transmit_playerData(player_getShots());
            speaker_playHit();

        }

    } 

    delay_start();
    nvic_globalirq_enable();
}

void game_new(void){
    enemy_start();
    player_start(DEFAULT_PLAYER_CODE);
    sender_start(DEFAULT_PLAYER_CODE);
    speaker_start();
    storage_start();
    batteryMonitor_initialize();
    gps_start();


    trigger_set_interrupt(game_triggerInterrupt);

    receiver_setInterrupt(game_receiverInterruptA);
    receiverB_setInterrupt(game_receiverInterruptB);

    //Some function to set the trigger button as an input, but not enable its interrupt.

}

void game_start(void){

    receiver_enable();
    receiverB_enable();

    receiver_start();
    receiverB_start();
    trigger_start();
    delay_init();

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

    //Sender.c does not turn off.

    //Speaker.c does not turn off.

    trigger_disable_interrupt();

    receiver_disable();

    receiverB_disable();

    gps_end();

    transmit_playerData(player_getShots());

    player_reset();

    enemy_reset();

    //Get all the remaining data and transmit it. 
    int i=0;
    for(i=0; i<storage_unsentEntries(); i++){
        hit toSend = storage_getShot();
        if(toSend.ID!=0){
            transmit_hitData(toSend);
            storage_removeHit(toSend.hitNumber);
        }
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
