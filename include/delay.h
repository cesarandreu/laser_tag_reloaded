#ifndef _DELAY_H_
#define _DELAY_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/exti.h>
#include <libmaple/timer.h>
#include "receiver.h"
#include "receiverBack.h"
#include "trigger.h"

void delay_defaultInterrupt();
void delay_setInterrupt(voidFuncPtr);
void delay_start();
void delay_init();

#ifdef __cplusplus
}
#endif

#endif 
