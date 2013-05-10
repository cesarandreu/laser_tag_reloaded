#include "receiverA.h"

#define RECEIVER_A_PORT GPIOA
#define RECEIVER_A_PIN 15
#define RECEIVER_A_EXTI_LINE AFIO_EXTI_15
#define RECEIVER_A_EXTI_PORT AFIO_EXTI_PA

uint16 receiverA_pulses[100][2] = {{0}};
uint8 receiverA_currentPulse = 0;
uint8 receiverA_numberPulses = 0;
uint8 receiverA_playerNumber = 1;

void receiverA_defaultInterrupt(void){
  //Does nothing
}

voidFuncPtr receiverA_handler = receiverA_defaultInterrupt;

int receiverA_listenForIR(void) {
  
  receiverA_currentPulse = 0;
 
  while (1) {
    uint16 highPulse, lowPulse;  // temporary storage timing
    highPulse = lowPulse = 0; // start out with no pulse length
 
      while(gpio_read_bit(RECEIVER_A_PORT, RECEIVER_A_PIN)){
       // pin is still HIGH
 
       // count off another few microseconds
       highPulse++;
       delay_us(RESOLUTION);
 
       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if ((highPulse >= MAXPULSE) && (receiverA_currentPulse != 0)) {
         return receiverA_currentPulse;
       }
    }
    // we didn't time out so lets stash the reading
    receiverA_pulses[receiverA_currentPulse][0] = highPulse;
 
    // same as above
    //while (! (IRpin_PIN & _BV(IRpin))) {
      while(!(gpio_read_bit(RECEIVER_A_PORT, RECEIVER_A_PIN))){
       // pin is still LOW
       lowPulse++;
       delay_us(RESOLUTION);
       if ((lowPulse >= MAXPULSE)  && (receiverA_currentPulse != 0)) {
         return receiverA_currentPulse;
       }
    }
    receiverA_pulses[receiverA_currentPulse][1] = lowPulse;
 
    // we read one high-low pulse successfully, continue!
    receiverA_currentPulse++;
    if (receiverA_currentPulse > WANTED_PULSES){
      return 0;
    }
  }
}

int receiverA_listenSignal(void){
    
    if(gpio_read_bit(RECEIVER_A_PORT, RECEIVER_A_PIN) == 0){

    receiverA_numberPulses = receiverA_listenForIR();
    receiverA_playerNumber = receiverA_interpretCode();

    //In theory, sets the player number to zero if the number of pulses does not fit with criteria.
    if((receiverA_numberPulses > WANTED_PULSES)||(receiverA_numberPulses < (WANTED_PULSES-1))){
      receiverA_playerNumber = 0;
    }
            
    return receiverA_playerNumber;

    } else {
      return 0;
    }
        
}

uint8 receiverA_interpretCode(void){
  uint8 code = 0;
  int i;
  for(i = 0; i<4 ; i++){
    if(receiverA_pulses[i][1] * RESOLUTION > ZERO_DURATION){
      code = code | (1 << (2*i));
    } 
    
    if(receiverA_pulses[i+1][0] * RESOLUTION > ZERO_DURATION){
        code = code | (1 << (2*i+1));
    }
    
  }
  return code;
}

void receiverA_start(void){
    gpio_set_mode(RECEIVER_A_PORT, RECEIVER_A_PIN, GPIO_INPUT_PU);
    exti_attach_interrupt(RECEIVER_A_EXTI_LINE, RECEIVER_A_EXTI_PORT, receiverA_handler, EXTI_FALLING);
}

void receiverA_end(void){
  exti_detach_interrupt(RECEIVER_A_EXTI_LINE);
}

void receiverA_setInterrupt(voidFuncPtr pointer){
  receiverA_handler = pointer;
}

void receiverA_disableInterrupt(void){
  exti_detach_interrupt(RECEIVER_A_EXTI_LINE);
}

void receiverA_enableInterrupt(void){
    exti_attach_interrupt(RECEIVER_A_EXTI_LINE, RECEIVER_A_EXTI_PORT, receiverA_handler, EXTI_FALLING);
}
