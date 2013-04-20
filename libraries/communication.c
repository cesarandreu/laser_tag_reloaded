
#include "communication.h"


void transmit_playerData(int shotsFired){
    char message[128] = { ' ' };
    sprintf(message, "{type: 'player', shotsFired: %d}", shotsFired);
    bluetooth_printString(message);

}

void transmit_hitData(hit transmit){
    char message[128] = { ' ' };
    sprintf(message, "{type: 'hit', id: %d, hitNumber: %d, gps: '%s'}", transmit.ID, transmit.hitNumber, transmit.location);
    bluetooth_printString(message);
}


// ********* TRANSMIT ENDS HERE **********************************


/*****************************************************
* The MCU's response to the stuff the bluetooth sends.
******************************************************/

void response_information(char gameType, int gameLimit, int enemyNumber){
    char message[128] = { ' ' };
    sprintf(message, "{type: 'response', response: 'information', gameType: '%c, gameLimit: %d, enemyNumber: %d}", gameType, gameLimit, enemyNumber);
    bluetooth_printString(message);
}

void response_hitAcknowledged(int hitNumber){
    char message[128] = { ' ' };
    sprintf(message, "{type: 'response', response: 'hitAcknowledged', hitNumber: %d}", hitNumber);
    bluetooth_printString(message);
}

void response_end(void){
    char message[128] = { ' ' };
    sprintf(message, "{type: 'response', response: 'end'}");
    bluetooth_printString(message);

}

void response_new(void){
    char message[128] = { ' ' };
    sprintf(message, "{type: 'response', response: 'new'}");
    bluetooth_printString(message);
}

void response_start(void){
    char message[128] = { ' ' };
    sprintf(message, "{type: 'response', response: 'start'}");
    bluetooth_printString(message);
}

// ********* RESPONSE ENDS HERE **********************************


//================================================================
// HOW DATA IS ENCODED TO BE SENT TO THE PHONE
// Must include a space ' ' at the end of the string.
// This is because of the way the interrupts are handled.
//================================================================
//Data acknowledge - #A,XXX,#
// Example: #A,128,#
// A   - Data acknowedleged
// XXX - hit number that was saved.

//Game end - #O,X,# 
// Example: #O,1,#
// O  - Game Over
// X  - If you won or not (1 TRUE, 0 FALSE)

//Game information - #I,A,Y1,00,X,X1,X2,X3,X4,X5,X6,X7,X8,# 
//Example: #I,A,128,10,2,5,10,# 
// I  - Game Info
// X  - Game Type (A = First to X Kills, B = Timed Deathmatch)
// Y1 - Player number
// 00 - Maximum number of kills/minutes
// X  - Number of enemies
// X1 - Enemy 1
// X2 - Enemy 2
// ...
// #  - End

//Game start - #S,#
// Example: #S,#
// S  - Game Start

//New  game - #N,#
// Example: #N,#
// N - New game

//================================================================


//To each receive_gameFUNCTION it might be a good idea to add an if at the start.
//Something like if(receive_endTag - receive_startTag < MINIMUM_ACCEPTED_VALUE_FOR_FUNC) return;

//Variables related to receiving something
//========================================
#define BUFFER_SIZE 128
#define CHAR_MAX 4
char receive_buffer[BUFFER_SIZE] = {' '};
int receive_position = 1;
int receive_startTag = 0;
int receive_endTag = 0;
char receive_nextChar[CHAR_MAX] = {' '};

int receive_getNext(int position){
    int counter = 0;
    int i = 0;

    for(i=0; i<CHAR_MAX; i++){
        receive_nextChar[i] = ' ';
    }
    receive_nextChar[CHAR_MAX-1] = '\0';

    while(receive_buffer[position]!=','){
        receive_nextChar[counter] = receive_buffer[position];
        position++;
        counter++;
    } 

    return position+1;
}

int receive_parseInt(void){
    return strtol(receive_nextChar, NULL, 10);  
}


/*************************************
* Some processing logic is below here. 
* Here is how we decide what we do when we receive stuff.
* This could blow up if the string is structured as expected.
**************************************/

void receive_gameNew(void){
    //Function that makes the micro reset everything for a new game.

    //Some function that responds to the micro saying that you've started a new game state.
    response_new();

    //Does something to start a new game.
    //game_new();

    /*
    //Testing:
    char myString[100] = "Game new!";
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */

}

void receive_gameOver(void){
    int gameResult = 0;
    int value = receive_startTag + 3;
    value = receive_getNext(value);
    gameResult = receive_parseInt();
    
    /*
    //Testing:
    char myString[100] = {' '};
    sprintf(myString, "\n %d result - game over!", gameResult);
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */

    /*
    if(gameResult==2){
        //If it's a TWO it means the game was quit or something else happened.
    } else if(gameResult==1){
        //Play winning sound
        //Do something to show game was won.
    } else { 
        //Play lose sound
        //Do something to show game was lost.
    }
    */

    //Some function to respond to the phone saying that the phone has acknowledged that the game has ended.
    response_end();

    //Does something now to end the game.
    //game_end(gameResult);

}

void receive_gameInformation(void){
    int value = receive_startTag + 3;
    int received_playerNumber = 0;
    char received_gameType = ' ';
    int received_gameLimit = 0;
    int received_enemyNumber = 0;

    //Gets the game type.
    //A = First to X Kills.
    //B = Timed Deathmatch
    value = receive_getNext(value);
    received_gameType = receive_nextChar[0];

    //Gets your player number.
    value = receive_getNext(value);
    received_playerNumber = receive_parseInt();

    //Gets the limiting factor of the game.
    //If A, kill limit.
    //If B, time limit.
    value = receive_getNext(value);
    received_gameLimit = receive_parseInt();

    //Gets the number of enemies.
    value = receive_getNext(value);
    received_enemyNumber = receive_parseInt();

    
    int i=0;
    //The MAXIMUM_NUMBER_ENEMIES variable is taken from enemies.h
    while(i<received_enemyNumber && i<=MAXIMUM_NUMBER_ENEMIES){
        i++;
        value = receive_getNext(value);

        //This function adds the number to the list of potential enemies. 
        enemy_addEnemy(receive_parseInt());
    }
    
    

    //Functions to save the player data
    //player_start(received_playerNumber);


    //Function that responds to the micro saying that the game info was saved
    response_information(received_gameType, received_gameLimit, received_enemyNumber);


    //Testing:
 /* value = receive_getNext(value);
    int enemy = receive_parseInt();
    char myString[256] = {' '};
    sprintf(myString, "\n %c Game Type, %d Player Number, %d Game Limit, %d Enemy Limit, %d Enemy \n", received_gameType, received_playerNumber, received_gameLimit, received_enemyNumber, enemy);
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);*/


}

void receive_gameAcknowledge(void){
    int numberAcknowledged = 0;
    int value = receive_startTag + 3; 
    value = receive_getNext(value);
    numberAcknowledged = receive_parseInt();

    //Remove acknowledged number.
    //storage_removeHit returns the hitNumber it removed.
    numberAcknowledged = storage_removeHit(numberAcknowledged);

    //Then responds to bluetooth saying it removed it.
    //If it wasn't removed for some reson, it'll return zero. 
    response_hitAcknowledged(numberAcknowledged);

    /*
    //Testing:
    char myString[100] = {' '};
    sprintf(myString, "\n %d number acknowledged!", numberAcknowledged);
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */

}

void receive_gameStart(void){
    //Function that responds to the micro saying that the game started.
    response_start();

    //Function that makes the game start. 
    //game_start();

    /*
    //Testing:
    char myString[100] = "Game start!";
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */

}

void receive_processString(void){
    char receive_messageType = ' ';

    if(receive_buffer[receive_startTag]!='#'){
        return;
    } 

    if(receive_buffer[receive_endTag]!='#'){
        return; 
    }


    receive_messageType = receive_buffer[receive_startTag+1];

    switch(receive_messageType){
        case 'S':
            receive_gameStart();
            break;
        case 'A':
            receive_gameAcknowledge();
            break;
        case 'O':
            receive_gameOver();
            break;
        case 'I':
            receive_gameInformation();
            break;
        case 'N':
            receive_gameNew();
            break;
        default:
            break;
    }

    //Resets the start and end tags. Sets the position to 1. 
    int i = 0;
    for(i=0; i<BUFFER_SIZE; i++){
        receive_buffer[i] = ' ';
    }
    receive_buffer[BUFFER_SIZE-1] = '\0';
    receive_startTag = 0;
    receive_endTag = 0;
    receive_position = 1;

}

void received_bluetooth(void){
    if(!bluetooth_available()){
        return;
    }

    while(bluetooth_available()){
        receive_buffer[receive_position] = bluetooth_read();
        if(receive_buffer[receive_position]=='#'){
            if(receive_startTag){
                receive_endTag=receive_position;
                break; 
            } else {
                receive_startTag=receive_position;
            }
        }

        //If the position is bigger or equal to the maximum size, it resets.
        if(receive_position>=BUFFER_SIZE){
            //Resets the start and end tags. Sets the position to 1. 
            int i = 0;
            for(i=0; i<BUFFER_SIZE; i++){
                receive_buffer[i] = ' ';
            }
            receive_buffer[BUFFER_SIZE-1] = '\0';
            receive_startTag = 0;
            receive_endTag = 0;
            receive_position = 1;
            break;
        }

        receive_position++;
    }


    if(receive_startTag!=0 && receive_endTag!=0){
        receive_processString();
    }
    
}

// ********* RECEIVE ENDS HERE **********************************


void communication_start(void){
    bluetooth_start();
    exti_attach_interrupt(BLUETOOTH_AFIO_EXTI_PIN, BLUETOOTH_AFIO_EXTI_PORT, received_bluetooth, EXTI_RISING);

}



