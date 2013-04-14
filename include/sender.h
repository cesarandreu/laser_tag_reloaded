#ifndef _SENDER_H_
#define _SENDER_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>

void sender_sendCode(uint8);
void sender_shoot(void);
void sender_pulseIR(long);
void sender_sendOneHigh(void);
void sender_sendZeroHigh(void);
void sender_sendOneLow(void);
void sender_sendZeroLow(void);
void sender_setPlayerCode(uint8);
uint8 sender_getPlayerCode(void);
void sender_start(int);


#ifdef __cplusplus
}
#endif

#endif 