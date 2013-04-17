#include "player.h"

int code = DEFAULT_PLAYER_CODE;
unsigned int shotsFired = DEFAULT_SHOTS_FIRED;

void player_start(int pCode){
    code = pCode;
    shotsFired = 0;
}

void player_shoot(void){
    shotsFired++;
}

unsigned int player_getShots(void){
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