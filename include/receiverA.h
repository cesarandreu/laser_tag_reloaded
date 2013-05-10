#ifndef _RECEIVERA_H_
#define _RECEIVERA_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include "receiverCommon.h"
#include <libmaple/libmaple.h>
#include <libmaple/exti.h>
#include <libmaple/gpio.h>
#include <libmaple/nvic.h>
#include <stdio.h>

int receiverA_listenForIR(void);
int receiverA_listenSignal(void);
uint8 receiverA_interpretCode(void);
void receiverA_start(void);
void receiverA_end(void);
void receiverA_setInterrupt(voidFuncPtr);
void receiverA_disableInterrupt(void);
void receiverA_enableInterrupt(void);
void receiverA_defaultInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif 