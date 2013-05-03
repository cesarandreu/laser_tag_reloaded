#include "player.h"

int code = DEFAULT_PLAYER_CODE;
volatile int shotsFired = DEFAULT_SHOTS_FIRED;

void player_start(int pCode){
    code = pCode;
    shotsFired = 0;
}

void player_shoot(void){
    //Checks if the number of shots fired is below 1 million. At one million shots it stops increasing.
    //If you shoot one million times in one game... You have problems. 
    if(shotsFired<1000000){
        shotsFired++;       
    }
}

int player_getShots(void){
    return shotsFired;
}

int player_getCode(void){
    return code;
}

void player_setCode(int pCode){
    code = pCode;
}

void player_reset(void){
    code = DEFAULT_PLAYER_CODE;
    shotsFired = DEFAULT_SHOTS_FIRED;
}
