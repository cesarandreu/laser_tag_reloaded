/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#include "trigger.h"
#include <libmaple/timer.h>

//#define TRIGGER_PORT GPIOC
//#define TRIGGER_PIN 14
#define TRIGGER_PORT GPIOA
#define TRIGGER_PIN 13

#define TRIGGER_TIMER TIMER2
#define TRIGGER_TIMER_CH TIMER_CH1
#define TRIGGER_PERIOD 125000

#define MAX_RELOAD ((1 << 16) - 1)

#define CYCLES_PER_MICROSECOND 72

void trigger_delay_start(void);
void trigger_delay_end(void);
uint16 trigger_setPeriod(uint32 microseconds);

void trigger_defaultInterrupt(void)
{   
    return;
}

voidFuncPtr trigger_handler = trigger_defaultInterrupt;

void trigger_start(void){
	timer_init(TRIGGER_TIMER);
	timer_pause(TRIGGER_TIMER);
	trigger_setPeriod(TRIGGER_PERIOD);
	timer_set_compare(TRIGGER_TIMER, TRIGGER_TIMER_CH, 1);
	timer_set_mode(TRIGGER_TIMER, TRIGGER_TIMER_CH, TIMER_OUTPUT_COMPARE);
	timer_attach_interrupt(TRIGGER_TIMER, TRIGGER_TIMER_CH, trigger_delay_end);
	timer_generate_update(TRIGGER_TIMER);
	timer_pause(TRIGGER_TIMER);

	gpio_set_mode(TRIGGER_PORT, TRIGGER_PIN, GPIO_INPUT_PD);
	//trigger_handler = trigger_defaultInterrupt;
	exti_attach_interrupt(AFIO_EXTI_13, AFIO_EXTI_PA, trigger_handler, EXTI_RISING);
}

void trigger_end(void){
	gpio_set_mode(TRIGGER_PORT, TRIGGER_PIN, GPIO_INPUT_FLOATING);
}

void trigger_set_interrupt(voidFuncPtr handler){
	trigger_handler = handler;
	exti_attach_interrupt(AFIO_EXTI_13, AFIO_EXTI_PA, trigger_handler, EXTI_RISING);
}

void trigger_enable_interrupt(void){
	exti_attach_interrupt(AFIO_EXTI_13, AFIO_EXTI_PA, trigger_handler, EXTI_RISING);
}

void trigger_disable_interrupt(void){
	exti_detach_interrupt(AFIO_EXTI_13);
}

//###TRIGGER DELAY IMPLEMENTATION###
uint16 trigger_setPeriod(uint32 microseconds){
    if(!microseconds){
        timer_set_prescaler(TRIGGER_TIMER, 1);
        timer_set_reload(TRIGGER_TIMER, 1);
        return timer_get_reload(TRIGGER_TIMER);
    }

    uint32 period_cyc = microseconds * CYCLES_PER_MICROSECOND;
    uint16 prescaler = (uint16)(period_cyc / MAX_RELOAD + 1);
    uint16 reload = (uint16)((period_cyc + (prescaler / 2)) / prescaler);
    timer_set_prescaler(TRIGGER_TIMER, prescaler);
    timer_set_reload(TRIGGER_TIMER, reload);
    return reload;
}

void trigger_delay_end(void){
	trigger_enable_interrupt();
	timer_pause(TRIGGER_TIMER);
}

void trigger_delay_start(void){
	trigger_disable_interrupt();
	timer_resume(TRIGGER_TIMER);
}