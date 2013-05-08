#include "delay.h"
#include "receiver.h"
#include "receiverBack.h"
#include "trigger.h"

#define DELAY_TIMER TIMER1
#define DELAY_TIMER_CH TIMER_CH2
#define DELAY_PERIOD 5000000

#define MAX_RELOAD ((1 << 16) - 1)

#define CYCLES_PER_MICROSECOND 72

void delay_defaultInterrupt(){
	trigger_enableInterrupt();
	receiver_enable();
	receiverB_enable();
	timer_pause(DELAY_TIMER);
}

voidFuncPtr delay_handler = &delay_defaultInterrupt;

void delay_setInterrupt(voidFuncPtr handler){
	delay_handler = handler;
	timer_attach_interrupt(DELAY_TIMER, DELAY_TIMER_CH, delay_handler);
}

uint16 delay_setPeriod(uint32 microseconds){
    if(!microseconds){
        timer_set_prescaler(DELAY_TIMER, 1);
        timer_set_reload(DELAY_TIMER, 1);
        return timer_get_reload(DELAY_TIMER);
    }

    uint32 period_cyc = microseconds * CYCLES_PER_MICROSECOND;
    uint16 prescaler = (uint16)(period_cyc / MAX_RELOAD + 1);
    uint16 reload = (uint16)((period_cyc + (prescaler / 2)) / prescaler);
    timer_set_prescaler(DELAY_TIMER, prescaler);
    timer_set_reload(DELAY_TIMER, reload);
    return reload;
}

void delay_init(){
	timer_init(DELAY_TIMER);
	timer_pause(DELAY_TIMER);
	delay_setPeriod(DELAY_PERIOD);
	timer_set_compare(DELAY_TIMER, DELAY_TIMER_CH, 1);
	timer_set_mode(DELAY_TIMER, DELAY_TIMER_CH, TIMER_OUTPUT_COMPARE);
	timer_attach_interrupt(DELAY_TIMER, DELAY_TIMER_CH, delay_handler);
	timer_generate_update(DELAY_TIMER);
}

void delay_start(){
	trigger_disableInterrupt();
	receiver_disable();
	receiverB_disable();
	timer_resume(DELAY_TIMER);
}