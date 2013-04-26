#ifndef _LEDGRAPH_H_
#define _LEDGRAPH_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/exti.h>
#include <libmaple/timer.h>

void writeLed(void);
void offLeds(void);
uint16 timer_setPeriod(uint32 microseconds);
void batteryMonitor_initialize(void);

#ifdef __cplusplus
}
#endif

#endif 