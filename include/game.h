
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
#include "receiver.h"
#include "receiverBack.h"
#include "receiverCommon.h"
#include "sender.h"
#include "speaker.h"
#include "storage.h"
#include "trigger.h"

void game_receiverInterrupt(void);
void game_new(void);
void game_start(void);
void game_information(char, int, int, int, int[]);
void game_end(int);

#ifdef __cplusplus
}
#endif

#endif 
