
#include "receiver.h"
#include "bluetooth.h"
#include "speaker.h"
#include <libmaple/libmaple.h>
#include <libmaple/exti.h>
#include <libmaple/gpio.h>
#include <libmaple/nvic.h>
#include <stdio.h>

#define ZERO_DURATION 541
#define MAXPULSE 65000
#define RESOLUTION 20
#define RECEIVER_PORT GPIOC
#define RECEIVER_PIN 13


uint16 pulses[100][2] = {{0}};
uint8 currentPulse = 0;
uint8 numberPulses = 0;
uint8 playerNumber = 0;

int listenForIR(void) {
  
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
  }
}

void listenSignal(void){
    //exti_detach_interrupt(AFIO_EXTI_15);
    
    if(gpio_read_bit(RECEIVER_PORT, RECEIVER_PIN) == 0){
    
    nvic_globalirq_disable();
    //gpio_toggle_bit(GPIOB, 1);
    numberPulses = listenForIR();
    playerNumber = interpretCode();
    //char pNum[40] = {'0'};
    //sprintf(pNum, "Num is: %d , numpulse is: %d", playerNumber, numberPulses);
    
    //bluetooth_printString(pNum);

    
    nvic_globalirq_enable();

    if(playerNumber==8){
      speaker_playHit();
    }

    //usb_printChar('R');
    //exti_attach_interrupt(AFIO_EXTI_15, AFIO_EXTI_PC, listenSignal, EXTI_FALLING);
        
    }
        
}

uint8 interpretCode(void){
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
    //afio_exti_select(AFIO_EXTI_15, AFIO_EXTI_PC);
    exti_attach_interrupt(AFIO_EXTI_13, AFIO_EXTI_PC, listenSignal, EXTI_FALLING);

}






