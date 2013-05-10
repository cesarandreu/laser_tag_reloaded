#ifndef _RECEIVERB_H_
#define _RECEIVERB_H_

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

int receiverB_listenForIR(void);
int receiverB_listenSignal(void);
uint8 receiverB_interpretCode(void);
void receiverB_start(void);
void receiverB_end(void);
void receiverB_setInterrupt(voidFuncPtr);
void receiverB_disableInterrupt(void);
void receiverB_enableInterrupt(void);
void receiverB_defaultInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif 