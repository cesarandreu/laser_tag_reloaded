#ifndef _RECEIVER_H_
#define _RECEIVER_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>

int receiver_listenForIR(void);
void receiver_listenSignal(void);
uint8 receiver_interpretCode(void);
void receiver_start(void);
void receiver_setInterrupt(voidFuncPtr);
void receiver_disable(void);
void receiver_enable(void);
    
#ifdef __cplusplus
}
#endif

#endif 