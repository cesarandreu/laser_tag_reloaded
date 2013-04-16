/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Bryan Newbold.
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


#include "speaker.h"
#include "initialize.h"
#include <libmaple/libmaple.h>
#include <libmaple/rcc.h>
#include <libmaple/timer.h>
#include <libmaple/gpio.h>

#define SPEAKER_PORT GPIOB
#define SPEAKER_PIN 0
#define SPEAKER_TIMER TIMER3
#define SPEAKER_CHANNEL TIMER_CH3
#define SPEAKER_COMPARE 1

//Value by which the count is modded to increase the period
#define SPEAKER_SHOOT_MOD 50
#define SPEAKER_HIT_MOD 100

//Amount in microseconds by which the period is increased
#define SPEAKER_HIT_INCREASE 100
#define SPEAKER_SHOOT_INCREASE 12

//Initial period in microseconds
#define SPEAKER_SHOOT_PERIOD 25
#define SPEAKER_HIT_PERIOD 800

//The number that count will reach before the sound stops
#define MAX_COUNT_SHOOT 1500 
#define MAX_COUNT_HIT 925



//Speaker variables
int count = 0;
unsigned int period = 0;


void speaker_pause(void){
    timer_pause(SPEAKER_TIMER);
}

void speaker_resume(void){
    timer_resume(SPEAKER_TIMER);
}

void speaker_setPrescaleFactor(uint32 factor){
    timer_set_prescaler(SPEAKER_TIMER, (uint16)(factor - 1));
}

void speaker_setOverflow(uint16 val){
    timer_set_reload(SPEAKER_TIMER, val);
}

uint16 speaker_getOverflow(void){
    return timer_get_reload(SPEAKER_TIMER);
}

void speaker_setCount(uint16 val){
    uint16 ovf = speaker_getOverflow();

    //Selects the minimum of the two
    uint16 result = ((val)<(ovf)?(val):(ovf)); 

    timer_set_count(SPEAKER_TIMER, result);
}

#define MAX_RELOAD ((1 << 16) - 1)
uint16 speaker_setPeriod(uint32 microseconds){
    if(!microseconds){
        speaker_setPrescaleFactor(1);
        speaker_setOverflow(1);
        return speaker_getOverflow();
    }

    uint32 period_cyc = microseconds * CYCLES_PER_MICROSECOND;
    uint16 prescaler = (uint16)(period_cyc / MAX_RELOAD + 1);
    uint16 overflow = (uint16)((period_cyc + (prescaler / 2)) / prescaler);
    speaker_setPrescaleFactor(prescaler);
    speaker_setOverflow(overflow);
    return overflow;
}

void speaker_setMode(int channel, timer_mode mode){
    timer_set_mode(SPEAKER_TIMER, (uint8)channel, (timer_mode)mode);
}

void speaker_setCompare(int channel, uint16 val){
    uint16 ovf = speaker_getOverflow();

    //Selects the minimum of the two
    uint16 result = ((val)<(ovf)?(val):(ovf)); 

    timer_set_compare(SPEAKER_TIMER, (uint8)channel, result);
}

void speaker_attachInterrupt(int channel, voidFuncPtr handler){
    timer_attach_interrupt(SPEAKER_TIMER, (uint8)channel, handler);
}


void speaker_detachInterrupt(int channel){
    timer_detach_interrupt(SPEAKER_TIMER, (uint8)channel);
}

void speaker_refresh(void){
    timer_generate_update(SPEAKER_TIMER);
}

 void speaker_start(void){

    gpio_set_mode(SPEAKER_PORT, SPEAKER_PIN, GPIO_OUTPUT_PP);

    timer_init(SPEAKER_TIMER);
    speaker_pause();
    speaker_setMode(SPEAKER_CHANNEL, TIMER_OUTPUT_COMPARE);
    speaker_setCompare(SPEAKER_CHANNEL, SPEAKER_COMPARE);
    speaker_refresh();
 }

void speaker_toggle(void){
    gpio_toggle_bit(SPEAKER_PORT, SPEAKER_PIN);
}

/*
void speaker_shoot(void){
    count++;
    speaker_toggle();
    if(count >= MAX_COUNT_SHOOT){
        speaker_pause();
        speaker_detachInterrupt(SPEAKER_CHANNEL);
        gpio_write_bit(SPEAKER_PORT, SPEAKER_PIN, 0);
    } else if( count % SPEAKER_SHOOT_MOD == 0 ){
        period += SPEAKER_SHOOT_INCREASE;
        speaker_setPeriod(period);
        speaker_refresh();
    }
}
*/

void speaker_shoot(void){
    count++;
    speaker_toggle();
    if(count >= MAX_COUNT_SHOOT){
        speaker_pause();
        speaker_detachInterrupt(SPEAKER_CHANNEL);
        gpio_write_bit(SPEAKER_PORT, SPEAKER_PIN, 0);
    } else if( count % SPEAKER_SHOOT_MOD == 0 ){
        period += SPEAKER_SHOOT_INCREASE;
        speaker_setPeriod(period);
        speaker_refresh();
    }
}

void speaker_hit(void){
    count++;
    speaker_toggle();
    if(count >= MAX_COUNT_HIT){
        speaker_pause();
        speaker_detachInterrupt(SPEAKER_CHANNEL);
        gpio_write_bit(SPEAKER_PORT, SPEAKER_PIN, 0);
    } else if( count % SPEAKER_HIT_MOD == 0 ){
        period += SPEAKER_HIT_INCREASE;
        speaker_setPeriod(period);
        speaker_refresh();
    }
}


void speaker_playHit(void){
    count = 0;
    period = SPEAKER_HIT_PERIOD;
    speaker_pause();
    speaker_setPeriod(SPEAKER_HIT_PERIOD);
    speaker_attachInterrupt(SPEAKER_CHANNEL, speaker_hit);
    speaker_refresh();
    speaker_resume();
}

void speaker_playShoot(void){
    count = 0;
    period = SPEAKER_SHOOT_PERIOD;
    speaker_pause();
    speaker_setPeriod(SPEAKER_SHOOT_PERIOD);
    speaker_attachInterrupt(SPEAKER_CHANNEL, speaker_shoot);
    speaker_refresh();
    speaker_resume();
}
