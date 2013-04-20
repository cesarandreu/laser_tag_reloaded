#include "receiver.h"
#include "speaker.h"
#include <libmaple/libmaple.h>
#include <libmaple/exti.h>
#include <libmaple/gpio.h>
#include <libmaple/nvic.h>
#include <stdio.h>

#define ZERO_DURATION 541
#define MAXPULSE 8125 // This value is 65000/8 (arbitrarily selected 8).
#define RESOLUTION 20
//Old
//#define RECEIVER_PORT GPIOC
//#define RECEIVER_PIN 13
#define RECEIVER_PORT GPIOA
#define RECEIVER_PIN 15
#define WANTED_PULSES 5
//Old
//#define RECEIVER_EXTI_LINE AFIO_EXTI_13
//#define RECEIVER_EXTI_PORT AFIO_EXTI_PC
#define RECEIVER_EXTI_LINE AFIO_EXTI_15
#define RECEIVER_EXTI_PORT AFIO_EXTI_PA
//#define RECEIVER_DEFAULT_HANDLER receiver_listenSignal


uint16 pulses[100][2] = {{0}};
uint8 currentPulse = 0;
uint8 numberPulses = 0;
uint8 playerNumber = 1;
voidFuncPtr handler = &receiver_defaultInterrupt;

void receiver_defaultInterrupt(void){
  receiver_listenSignal();
}

int receiver_listenForIR(void) {
  
  currentPulse = 0;
 
  while (1) {
    uint16 highPulse, lowPulse;  // temporary storage timing
    highPulse = lowPulse = 0; // start out with no pulse length
 
      while(gpio_read_bit(RECEIVER_PORT, RECEIVER_PIN)){
       // pin is still HIGH
 
       // count off another few microseconds
       highPulse++;
       delay_us(RESOLUTION);
 
       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if ((highPulse >= MAXPULSE) && (currentPulse != 0)) {
         return currentPulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentPulse][0] = highPulse;
 
    // same as above
    //while (! (IRpin_PIN & _BV(IRpin))) {
      while(!(gpio_read_bit(RECEIVER_PORT, RECEIVER_PIN))){
       // pin is still LOW
       lowPulse++;
       delay_us(RESOLUTION);
       if ((lowPulse >= MAXPULSE)  && (currentPulse != 0)) {
         return currentPulse;
       }
    }
    pulses[currentPulse][1] = lowPulse;
 
    // we read one high-low pulse successfully, continue!
    currentPulse++;
    if (currentPulse > WANTED_PULSES){
      return 0;
    }
  }
}

int receiver_listenSignal(void){
    
    if(gpio_read_bit(RECEIVER_PORT, RECEIVER_PIN) == 0){
    
    nvic_globalirq_disable();

    numberPulses = receiver_listenForIR();
    playerNumber = receiver_interpretCode();


    //In theory, sets the player number to zero if the number of pulses does not fit with criteria.
    if((numberPulses > WANTED_PULSES)||(numberPulses < (WANTED_PULSES-1))){
      playerNumber = 0;
    }
        
    nvic_globalirq_enable();
    
    return playerNumber;

    } else {
      return 0;
    }
        
}

uint8 receiver_interpretCode(void){
  uint8 code = 0;
  int i;
  for(i = 0; i<4 ; i++){
    if(pulses[i][1] * RESOLUTION > ZERO_DURATION){
      code = code | (1 << (2*i));
    } 
    
    if(pulses[i+1][0] * RESOLUTION > ZERO_DURATION){
        code = code | (1 << (2*i+1));
    }
    
  }
  return code;
}

void receiver_start(void){
    //gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_set_mode(RECEIVER_PORT, RECEIVER_PIN, GPIO_INPUT_PU);
    //afio_exti_select(RECEIVER_EXTI_LINE, RECEIVER_EXTI_PORT);
    exti_attach_interrupt(RECEIVER_EXTI_LINE, RECEIVER_EXTI_PORT, handler, EXTI_FALLING);

    
}


void receiver_setInterrupt(voidFuncPtr pointer){
  handler = pointer;
}

void receiver_disable(void){
  exti_detach_interrupt(RECEIVER_EXTI_LINE);
}

void receiver_enable(void){
    exti_attach_interrupt(RECEIVER_EXTI_LINE, RECEIVER_EXTI_PORT, handler, EXTI_FALLING);
}
