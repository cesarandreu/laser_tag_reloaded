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

//#define TRIGGER_PORT GPIOC
//#define TRIGGER_PIN 14
#define TRIGGER_PORT GPIOA
#define TRIGGER_PIN 13

voidFuncPtr trigger_handler = trigger_defaultInterrupt;

void trigger_defaultInterrupt(void)
{	
	return;
}

void trigger_start(void){
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