#include "receiverBack.h"
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
//#define RECEIVERB_PORT GPIOC
//#define RECEIVERB_PIN 13
#define RECEIVERB_PORT GPIOA
#define RECEIVERB_PIN 15
#define WANTED_PULSES 5
//Old
//#define RECEIVERB_EXTI_LINE AFIO_EXTI_13
//#define RECEIVERB_EXTI_PORT AFIO_EXTI_PC
#define RECEIVERB_EXTI_LINE AFIO_EXTI_15
#define RECEIVERB_EXTI_PORT AFIO_EXTI_PA
//#define RECEIVERB_DEFAULT_HANDLER RECEIVERB_listenSignal


uint16 pulsesB[100][2] = {{0}};
uint8 currentPulseB = 0;
uint8 numberPulsesB = 0;
uint8 playerNumberB = 1;
voidFuncPtr handlerB = &receiverB_defaultInterrupt;

void receiverB_defaultInterrupt(void){
  receiverB_listenSignal();
}

int receiverB_listenForIR(void) {
  
  currentPulseB = 0;
 
  while (1) {
    uint16 highPulse, lowPulse;  // temporary storage timing
    highPulse = lowPulse = 0; // start out with no pulse length
 
      while(gpio_read_bit(RECEIVERB_PORT, RECEIVERB_PIN)){
       // pin is still HIGH
 
       // count off another few microseconds
       highPulse++;
       delay_us(RESOLUTION);
 
       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if ((highPulse >= MAXPULSE) && (currentPulseB != 0)) {
         return currentPulseB;
       }
    }
    // we didn't time out so lets stash the reading
    pulsesB[currentPulseB][0] = highPulse;
 
    // same as above
    //while (! (IRpin_PIN & _BV(IRpin))) {
      while(!(gpio_read_bit(RECEIVERB_PORT, RECEIVERB_PIN))){
       // pin is still LOW
       lowPulse++;
       delay_us(RESOLUTION);
       if ((lowPulse >= MAXPULSE)  && (currentPulseB != 0)) {
         return currentPulseB;
       }
    }
    pulsesB[currentPulseB][1] = lowPulse;
 
    // we read one high-low pulse successfully, continue!
    currentPulseB++;
    if (currentPulseB > WANTED_PULSES){
      return 0;
    }
  }
}

int receiverB_listenSignal(void){
    
    if(gpio_read_bit(RECEIVERB_PORT, RECEIVERB_PIN) == 0){
    
    nvic_globalirq_disable();

    numberPulsesB = receiverB_listenForIR();
    playerNumberB = receiverB_interpretCode();


    //In theory, sets the player number to zero if the number of pulses does not fit with criteria.
    if((numberPulsesB > WANTED_PULSES)||(numberPulsesB < (WANTED_PULSES-1))){
      playerNumberB = 0;
    }
        
    nvic_globalirq_enable();
    
    return playerNumberB;

    } else {
      return 0;
    }
        
}

uint8 receiverB_interpretCode(void){
  uint8 code = 0;
  int i;
  for(i = 0; i<4 ; i++){
    if(pulsesB[i][1] * RESOLUTION > ZERO_DURATION){
      code = code | (1 << (2*i));
    } 
    
    if(pulsesB[i+1][0] * RESOLUTION > ZERO_DURATION){
        code = code | (1 << (2*i+1));
    }
    
  }
  return code;
}

void receiverB_start(void){
    gpio_set_mode(RECEIVERB_PORT, RECEIVERB_PIN, GPIO_INPUT_PU);
    //I don't think we need to use afio (alternate function i/o) here, unless the reciever
    //is operating as something other than a GPIO
    //afio_exti_select(RECEIVERB_EXTI_LINE, RECEIVERB_EXTI_PORT);
    exti_attach_interrupt(RECEIVERB_EXTI_LINE, RECEIVERB_EXTI_PORT, handlerB, EXTI_FALLING);

    
}


void receiverB_setInterrupt(voidFuncPtr pointer){
  handlerB = pointer;
}

void receiverB_disable(void){
  exti_detach_interrupt(RECEIVERB_EXTI_LINE);
}

void receiverB_enable(void){
    exti_attach_interrupt(RECEIVERB_EXTI_LINE, RECEIVERB_EXTI_PORT, handlerB, EXTI_FALLING);
}
