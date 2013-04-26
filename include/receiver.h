#ifndef _RECEIVER_H_
#define _RECEIVER_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>
#include <libmaple/exti.h>
#include <libmaple/gpio.h>
#include <libmaple/nvic.h>
#include <stdio.h>
#include "receiver.h"
#include "speaker.h"
#include "receiverCommon.h"

int receiver_listenForIR(void);
int receiver_listenSignal(void);
uint8 receiver_interpretCode(void);
void receiver_start(void);
void receiver_setInterrupt(voidFuncPtr);
void receiver_disable(void);
void receiver_enable(void);
void receiver_defaultInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif 