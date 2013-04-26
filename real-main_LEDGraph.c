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

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"
#include <libmaple/gpio.h>
#include <libmaple/exti.h>
#include <libmaple/timer.h>

#define MAX_RELOAD ((1 << 16) - 1)
#define BATTERY_BUTTON_PORT GPIOC
#define BATTERY_BUTTON_PIN 13
#define BATTERY_BUTTON_EXTI_LINE AFIO_EXTI_13
#define BATTERY_BUTTON_EXTI_PORT AFIO_EXTI_PC

#define BATTERY_LED1_PORT GPIOB
#define BATTERY_LED2_PORT GPIOB
#define BATTERY_LED3_PORT GPIOB
#define BATTERY_LED4_PORT GPIOA
#define BATTERY_LED5_PORT GPIOA
#define BATTERY_LED6_PORT GPIOA
#define BATTERY_LED7_PORT GPIOA
#define BATTERY_LED8_PORT GPIOC
#define BATTERY_LED9_PORT GPIOA
#define BATTERY_LED10_PORT GPIOB

#define BATTERY_LED1_PIN 11
#define BATTERY_LED2_PIN 10
#define BATTERY_LED3_PIN 2
#define BATTERY_LED4_PIN 7
#define BATTERY_LED5_PIN 6
#define BATTERY_LED6_PIN 5
#define BATTERY_LED7_PIN 4
#define BATTERY_LED8_PIN 15
#define BATTERY_LED9_PIN 14
#define BATTERY_LED10_PIN 3

#define BATTERY_TIMER TIMER1
#define BATTERY_TIMER_CH TIMER_CH1
#define BATTERY_PERIOD 2000000

void writeLed(void){
	int percent = gauge_percentFloat();
	if(percent>=100){
		gpio_write_bit(BATTERY_LED10_PORT, BATTERY_LED10_PIN,1);
		gpio_write_bit(BATTERY_LED9_PORT, BATTERY_LED9_PIN,1);
		gpio_write_bit(BATTERY_LED8_PORT, BATTERY_LED8_PIN,1);
		gpio_write_bit(BATTERY_LED7_PORT, BATTERY_LED7_PIN,1);
		gpio_write_bit(BATTERY_LED6_PORT, BATTERY_LED6_PIN,1);
		gpio_write_bit(BATTERY_LED5_PORT, BATTERY_LED5_PIN,1);
		gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,1);
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=90){
		gpio_write_bit(BATTERY_LED9_PORT, BATTERY_LED9_PIN,1);
		gpio_write_bit(BATTERY_LED8_PORT, BATTERY_LED8_PIN,1);
		gpio_write_bit(BATTERY_LED7_PORT, BATTERY_LED7_PIN,1);
		gpio_write_bit(BATTERY_LED6_PORT, BATTERY_LED6_PIN,1);
		gpio_write_bit(BATTERY_LED5_PORT, BATTERY_LED5_PIN,1);
		gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,1);
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=80){
		gpio_write_bit(BATTERY_LED8_PORT, BATTERY_LED8_PIN,1);
		gpio_write_bit(BATTERY_LED7_PORT, BATTERY_LED7_PIN,1);
		gpio_write_bit(BATTERY_LED6_PORT, BATTERY_LED6_PIN,1);
		gpio_write_bit(BATTERY_LED5_PORT, BATTERY_LED5_PIN,1);
		gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,1);
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=70){
		gpio_write_bit(BATTERY_LED7_PORT, BATTERY_LED7_PIN,1);
		gpio_write_bit(BATTERY_LED6_PORT, BATTERY_LED6_PIN,1);
		gpio_write_bit(BATTERY_LED5_PORT, BATTERY_LED5_PIN,1);
		gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,1);
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=60){
		gpio_write_bit(BATTERY_LED6_PORT, BATTERY_LED6_PIN,1);
		gpio_write_bit(BATTERY_LED5_PORT, BATTERY_LED5_PIN,1);
		gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,1);
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=50){
		gpio_write_bit(BATTERY_LED5_PORT, BATTERY_LED5_PIN,1);
		gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,1);
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=40){
		gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,1);
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=30){
		gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,1);
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=20){
		gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,1);
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}else if(percent>=10){
		gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,1);
	}
	timer_resume(BATTERY_TIMER);
}

void offLeds(void){
	gpio_write_bit(BATTERY_LED10_PORT, BATTERY_LED10_PIN,0);
	gpio_write_bit(BATTERY_LED9_PORT, BATTERY_LED9_PIN,0);
	gpio_write_bit(BATTERY_LED8_PORT, BATTERY_LED8_PIN,0);
	gpio_write_bit(BATTERY_LED7_PORT, BATTERY_LED7_PIN,0);
	gpio_write_bit(BATTERY_LED6_PORT, BATTERY_LED6_PIN,0);
	gpio_write_bit(BATTERY_LED5_PORT, BATTERY_LED5_PIN,0);
	gpio_write_bit(BATTERY_LED4_PORT, BATTERY_LED4_PIN,0);
	gpio_write_bit(BATTERY_LED3_PORT, BATTERY_LED3_PIN,0);
	gpio_write_bit(BATTERY_LED2_PORT, BATTERY_LED2_PIN,0);
	gpio_write_bit(BATTERY_LED1_PORT, BATTERY_LED1_PIN,0);
	timer_pause(BATTERY_TIMER);
}

uint16 timer_setPeriod(uint32 microseconds){
    if(!microseconds){
        timer_set_prescaler(BATTERY_TIMER, 1);
        timer_set_reload(BATTERY_TIMER, 1);
        return timer_get_reload(BATTERY_TIMER);
    }

    uint32 period_cyc = microseconds * CYCLES_PER_MICROSECOND;
    uint16 prescaler = (uint16)(period_cyc / MAX_RELOAD + 1);
    uint16 reload = (uint16)((period_cyc + (prescaler / 2)) / prescaler);
    timer_set_prescaler(BATTERY_TIMER, prescaler);
    timer_set_reload(BATTERY_TIMER, reload);
    return reload;
}

void batteryMonitor_initialize(void) {

	gauge_start(10);

	gpio_set_mode(BATTERY_LED1_PORT, BATTERY_LED1_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED2_PORT, BATTERY_LED2_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED3_PORT, BATTERY_LED3_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED4_PORT, BATTERY_LED4_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED5_PORT, BATTERY_LED5_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED6_PORT, BATTERY_LED6_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED7_PORT, BATTERY_LED7_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED8_PORT, BATTERY_LED8_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED9_PORT, BATTERY_LED9_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_LED10_PORT, BATTERY_LED10_PIN,GPIO_OUTPUT_PP);
	gpio_set_mode(BATTERY_BUTTON_PORT,BATTERY_BUTTON_PIN,GPIO_INPUT_PU);
	exti_attach_interrupt(BATTERY_BUTTON_EXTI_LINE, BATTERY_BUTTON_EXTI_PORT, writeLed, EXTI_RISING);

	timer_init(BATTERY_TIMER);
	timer_pause(BATTERY_TIMER);
	timer_setPeriod(BATTERY_PERIOD);
	timer_set_compare(BATTERY_TIMER, BATTERY_TIMER_CH, 1);
	timer_set_mode(BATTERY_TIMER, BATTERY_TIMER_CH, TIMER_OUTPUT_COMPARE);
	timer_attach_interrupt(BATTERY_TIMER, BATTERY_TIMER_CH, offLeds);
	timer_generate_update(BATTERY_TIMER);	
    gpio_write_bit(BATTERY_LED10_PORT, BATTERY_LED10_PIN,1);
}

void setup(void) {
    batteryMonitor_initialize();
}

void loop(void) {
}


__attribute__((constructor)) void premain() {
    init();
}

int main(void) {
    setup();

    while (1) {
        loop();
    }

    return 0;
}