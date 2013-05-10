
#include "game.h"
#include "communication.h"
#include "enemy.h"
#include "GPS.h"
#include "player.h"
#include "receiverA.h"
#include "sender.h"
#include "storage.h"
#include "trigger.h"



void game_triggerButton(void){

    player_shoot();
    sender_shoot();
    speaker_playShoot();

}


void game_receiverA_Interrupt(void){

    int receivedCode = receiverA_listenSignal();

    //Should use GPS_COORDINATE_LENGTH
    char gps_location[25] = "GPS_DATA_NOT_VALID_SORRY";

    if(receivedCode>0 && receivedCode<=256){

        if(enemy_checkExist(receivedCode)!=0){

        //Some function to turn on GPS and the location data.
        //gps_getLocation(gps_location);
        storage_add(receivedCode, gps_location);
        transmit_hitData(storage_getShot());
        transmit_playerData(player_getShots());
        speaker_playHit();

        //Some function that sets up a timer and disables the received and shooter.
        //Then it waits like 5~ seconds.
        //After the 5 seconds are up, it enables the received again and shooter. 
        }

    }

}

void game_new(void){
    enemy_start();
    player_start(DEFAULT_PLAYER_CODE);
    sender_start(DEFAULT_PLAYER_CODE);
    speaker_start();
    storage_start();

    trigger_set_interrupt(game_triggerButton);
    receiverA_setInterrupt(game_receiverA_Interrupt);

}

void game_start(void){
    trigger_start();
    receiverA_start();

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

    trigger_end();

    receiverA_end();
    

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

