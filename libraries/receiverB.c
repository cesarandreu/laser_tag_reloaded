#include "receiverB.h"

#define RECEIVER_B_PORT GPIOB
#define RECEIVER_B_PIN 5
#define RECEIVER_B_EXTI_LINE AFIO_EXTI_5
#define RECEIVER_B_EXTI_PORT AFIO_EXTI_PB

uint16 receiverB_pulses[100][2] = {{0}};
uint8 receiverB_currentPulse = 0;
uint8 receiverB_numberPulses = 0;
uint8 receiverB_playerNumber = 1;

void receiverB_defaultInterrupt(void){
  //Does nothing
}

voidFuncPtr receiverB_handler = receiverB_defaultInterrupt;

int receiverB_listenForIR(void) {
  
  receiverB_currentPulse = 0;
 
  while (1) {
    uint16 highPulse, lowPulse;  // temporary storage timing
    highPulse = lowPulse = 0; // start out with no pulse length
 
      while(gpio_read_bit(RECEIVER_B_PORT, RECEIVER_B_PIN)){
       // pin is still HIGH
 
       // count off another few microseconds
       highPulse++;
       delay_us(RESOLUTION);
 
       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if ((highPulse >= MAXPULSE) && (receiverB_currentPulse != 0)) {
         return receiverB_currentPulse;
       }
    }
    // we didn't time out so lets stash the reading
    receiverB_pulses[receiverB_currentPulse][0] = highPulse;
 
    // same as above
    //while (! (IRpin_PIN & _BV(IRpin))) {
      while(!(gpio_read_bit(RECEIVER_B_PORT, RECEIVER_B_PIN))){
       // pin is still LOW
       lowPulse++;
       delay_us(RESOLUTION);
       if ((lowPulse >= MAXPULSE)  && (receiverB_currentPulse != 0)) {
         return receiverB_currentPulse;
       }
    }
    receiverB_pulses[receiverB_currentPulse][1] = lowPulse;
 
    // we read one high-low pulse successfully, continue!
    receiverB_currentPulse++;
    if (receiverB_currentPulse > WANTED_PULSES){
      return 0;
    }
  }
}

int receiverB_listenSignal(void){
    
    if(gpio_read_bit(RECEIVER_B_PORT, RECEIVER_B_PIN) == 0){

    receiverB_numberPulses = receiverB_listenForIR();
    receiverB_playerNumber = receiverB_interpretCode();

    //In theory, sets the player number to zero if the number of pulses does not fit with criteria.
    if((receiverB_numberPulses > WANTED_PULSES)||(receiverB_numberPulses < (WANTED_PULSES-1))){
      receiverB_playerNumber = 0;
    }
            
    return receiverB_playerNumber;

    } else {
      return 0;
    }
        
}

uint8 receiverB_interpretCode(void){
  uint8 code = 0;
  int i;
  for(i = 0; i<4 ; i++){
    if(receiverB_pulses[i][1] * RESOLUTION > ZERO_DURATION){
      code = code | (1 << (2*i));
    } 
    
    if(receiverB_pulses[i+1][0] * RESOLUTION > ZERO_DURATION){
        code = code | (1 << (2*i+1));
    }
    
  }
  return code;
}

void receiverB_start(void){
    gpio_set_mode(RECEIVER_B_PORT, RECEIVER_B_PIN, GPIO_INPUT_PU);
    exti_attach_interrupt(RECEIVER_B_EXTI_LINE, RECEIVER_B_EXTI_PORT, receiverB_handler, EXTI_FALLING);
}

void receiverB_end(void){
  exti_detach_interrupt(RECEIVER_B_EXTI_LINE);
}

void receiverB_setInterrupt(voidFuncPtr pointer){
  receiverB_handler = pointer;
}

void receiverB_disableInterrupt(void){
  exti_detach_interrupt(RECEIVER_B_EXTI_LINE);
}

void receiverB_enableInterrupt(void){
    exti_attach_interrupt(RECEIVER_B_EXTI_LINE, RECEIVER_B_EXTI_PORT, receiverB_handler, EXTI_FALLING);
}
