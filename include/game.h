
#ifndef _GAME_H_
#define _GAME_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include "communication.h"
#include "enemy.h"
#include "GPS.h"
#include "player.h"
#include "receiverA.h"
#include "receiverB.h"
#include "sender.h"
#include "storage.h"
#include "trigger.h"
#include "delay.h"


void game_triggerButton(void);
void game_receiverA_Interrupt(void);
void game_receiverB_Interrupt(void);

void game_receiverInterrupt(void);
void game_new(void);
void game_start(void);
void game_information(char, int, int, int, int[]);
void game_end(int);

#ifdef __cplusplus
}
#endif

#endif 
