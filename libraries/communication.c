
#include "communication.h"


// TO-DO: 
// Add error responses.
// It should respond with known errors when possible.
// For example: Bad player number, game type, etc.
// This could be the return from the game functions.

// TO-DO: 
// Add response when something is just wrong.
// It should respond with as much information as possible.
// Maybe saying what type of function it was in, or something.
// Or what might've caused it.
// For example: Missing the comma before the #.

// TO-DO:
// Right now you can change the game state however you want.
// Doing so may cause conflicts (like doing game_over before game_new).
// So here or in game.c, there has to be some sort of check or failsafe.
// Either to not allow a certain function to execute.
// Unless a certain other function was called first.
// Or make it so that all game state functions can be called separately.
//Example:
//If New Game has not been run we cannot run Game Start, Game Over, Game Information.
//If Game Information has not been done we cannot run Game Start, Game Over.
//If Game Start has not been run we cannot run Game Over.

void transmit_playerData(int shotsFired){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"player\", \"shotsFired\": %d}\r\n", shotsFired);
    bluetooth_printString(message);

}

void transmit_hitData(hit transmit){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"hit\", \"id\": %d, \"hitNumber\": %d, \"gps\": \"%s\"}\r\n", transmit.ID, transmit.hitNumber, transmit.location);
    bluetooth_printString(message);
}


// ********* TRANSMIT ENDS HERE **********************************


/*****************************************************
* The MCU's response to the stuff the bluetooth sends.
******************************************************/

void response_information(char gameType, int playerNumber, int gameLimit, int enemyNumber){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"response\", \"response\": \"information\", \"gameType\": \"%c\", \"playerNumber\": %d, \"gameLimit\": %d, \"enemyNumber\": %d}\r\n", gameType, playerNumber, gameLimit, enemyNumber);
    bluetooth_printString(message);
}

void response_hitAcknowledged(int hitNumber){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"response\", \"response\": \"hitAcknowledged\", \"hitNumber\": %d}\r\n", hitNumber);
    bluetooth_printString(message);
}

void response_end(int resultNumber){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"response\", \"response\": \"end\", \"result\": %d}\r\n", resultNumber);
    bluetooth_printString(message);

}

void response_new(void){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"response\", \"response\": \"new\"}\r\n");
    bluetooth_printString(message);
}

void response_start(void){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"response\", \"response\": \"start\"}\r\n");
    bluetooth_printString(message);
}

void response_connected(void){
    char message[128] = { ' ' };
    sprintf(message, "{\"type\": \"response\", \"response\": \"connected\"}\r\n");
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

//New game - #N,#
// Example: #N,#
// N - New game

//Check connection - #C,#
//Example: #C,#
// C - Check status

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
        if(counter>(CHAR_MAX-1)){
            break;
        }
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

//Sets the game status to new game.
//Example: #N,#
//(4-1 = 3), this means it must be exactly three.
void receive_gameNew(void){
    
    //The received string must be exactly four characters long.
    //Since we do end-start then the length requires subtracting one.
    //If it's bigger or smaller, then it returns.
    if((receive_endTag-receive_startTag)<3 || (receive_endTag-receive_startTag)>3){
        return;
    }
    

    //Function that makes the micro reset everything for a new game.
    game_new();

    //Some function that responds to the micro saying that you've started a new game state.
    response_new();
    
    /*
    //Testing:
    char myString[100] = "Game new!";
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */

}

//Sets the game status to game over.
//Example: #O,X,# (#O,1,#)
//(6-1 = 5), this means it must be exactly five.
//This means there can be ten game over states.
void receive_gameOver(void){
    int gameResult = 0;
    int value = receive_startTag + 3;
    
    //The received string must be exactly six characters long.
    //Since we do end-start then the length requires subtracting one.
    //If it's bigger or smaller, then it returns.
    if((receive_endTag-receive_startTag)<5 || (receive_endTag-receive_startTag)>5){
        return;
    }
    
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
    //This logic was moved over to the game library.
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

    //Does something now to end the game.
    game_end(gameResult);
    
    //Function to respond to the phone saying that the micro has acknowledged that the game has ended.
    response_end(gameResult);

}

//Sets the game information for a game.
//Encoding: #I,T,XXP,XXL,XXE,XEN,#
//Shortest Possible Example: #I,A,8,5,1,4,#
//(14 - 1) = 13, so it must be at least thirteen long.
//Short Example: #I,A,128,10,1,16,#
//Longer Example: #I,B,128,100,2,16,32,#
//Explanation: 
// I    - Information
// T    - Game Type (A or B)
//          -> B = Timed Deathmatch
//          -> A = First to X Kills
// XXP  - Player Number [2-MAXIMUM_PLAYER_NUMBER], limited to three characters.
// XXL  - Game Limit, limited to three characters.
// XXE  - Number of enemies [1-MAXIMUM_NUMBER_ENEMIES], limited to three characters.
// XEN  - Enemy N, starting from zero to N-1
void receive_gameInformation(void){
    int value = receive_startTag + 3;
    int received_playerNumber = 0;
    char received_gameType = ' ';
    int received_gameLimit = 0;
    int received_enemyNumber = 0;
    int received_enemyList[MAXIMUM_NUMBER_ENEMIES] = {0};
    
    //The received string must be at least four characters long.
    //Since we do end-start then the length requires subtracting one.
    //If it's smaller than 13, then it returns.
    if((receive_endTag-receive_startTag)<13){
        return;
    }
    
    //Gets the game type.
    //A = First to X Kills.
    //B = Timed Deathmatch
    value = receive_getNext(value);
    received_gameType = receive_nextChar[0];
    
    /*
    // Move to game.c later on.
    //If the gameType is anything other than A or B, then it returns.
    if( received_gameType!='A' && received_gameType!='B' ){
        return;
    }
    */

    //Gets your player number.
    value = receive_getNext(value);
    received_playerNumber = receive_parseInt();
    
    /*
    // Move to game.c later on.
    //If the player number if above or under the limit, it returns.
    if( received_playerNumber>MAXIMUM_PLAYER_NUMBER || received_playerNumber<MINIMUM_PLAYER_NUMBER ){
        return;
    }
    */

    //Gets the limiting factor of the game.
    //If A, kill limit.
    //If B, time limit.
    value = receive_getNext(value);
    received_gameLimit = receive_parseInt();

    //Gets the number of enemies.
    value = receive_getNext(value);
    received_enemyNumber = receive_parseInt();
    
    /*
    // Move to game.c later on.
    //If the number of enemies is above or under the limit, it returns.
    if( received_enemyNumber>MAXIMUM_NUMBER_ENEMIES || received_enemyNumber<1 ){
    return;
    }
    */
    

    int i=0;
    for(i=0; i<received_enemyNumber; i++){
        value = receive_getNext(value);
        received_enemyList[i] = receive_parseInt();

        /*
        // Each enemy requires at LEAST one digit and one comma.
        // So by i from the enemyNumber, we find how many players are left.
        // We multiply by two and we have the minimum number of spaces.
        // value is our current position starting from the number
        // So if we subtract our position (value) from endTag
        // We get the number of spaces left ( + 1 for the hashtag)
        // So if the characters needed is greater than the characters we have
        // That means that something is wrong.
        if( ( (received_enemyNumber - i) * 2 ) > (receive_endTag - value) ){
            return;
        }
        */

    }
    
    /*
    //Deprecated, moved this to game.c
    //The MAXIMUM_NUMBER_ENEMIES variable is taken from enemies.h
    while(i<received_enemyNumber && i<=MAXIMUM_NUMBER_ENEMIES){
        i++;
        value = receive_getNext(value);

        //This function adds the number to the list of potential enemies. 
        enemy_addEnemy(receive_parseInt());
    }
    */



    //Function to store the game information.
    game_information(received_gameType, received_playerNumber, received_gameLimit, received_enemyNumber, received_enemyList);

    //Function that responds to the micro saying that the game info was saved
    response_information(received_gameType, received_playerNumber, received_gameLimit, received_enemyNumber);

    //Testing:
    /*
    value = receive_getNext(value);
    int enemy = receive_parseInt();
    char myString[256] = {' '};
    sprintf(myString, "\n %c Game Type, %d Player Number, %d Game Limit, %d Enemy Limit, %d Enemy \n", received_gameType, received_playerNumber, received_gameLimit, received_enemyNumber, enemy);
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */


}

//Checks connection
//Example: #C,#
//(4-1 = 3),  so it must be exactly three.
void receive_connected(void){
    
    //The received string must be exactly four characters long.
    //Since we do end-start then the length requires subtracting one.
    //If it's bigger or smaller, then it returns.
    if((receive_endTag-receive_startTag)<3 || (receive_endTag-receive_startTag)>3){
        return;
    }

    //Function that responds that the MCU is connected.
    response_connected();

}

//Starts the game!
//Example: #S,#
//(4-1 = 3),  so it must be exactly three.
void receive_gameStart(void){
    
    //The received string must be exactly four characters long.
    //Since we do end-start then the length requires subtracting one.
    //If it's bigger or smaller, then it returns.
    if((receive_endTag-receive_startTag)<3 || (receive_endTag-receive_startTag)>3){
        return;
    }
    

    //Function that makes the game start. 
    game_start();

    //Function that responds saying that the game start has been acknowledged.
    response_start();
    
    /*
    //Testing:
    char myString[100] = "Game start!";
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */

}
    
//Removes the acknowledged number from storage.
//Shortest possible example: #A,X,# (#A,1,#)
//Longest possible example: #A,XXX,# (#A,128,#)
//(6-1 = 5), (8-1 = 7), so it must be bigger than five and smaller than eight.
//This means that shot numbers cannot surpass 999 per player.
void receive_gameAcknowledge(void){
    int numberAcknowledged = 0;
    int value = receive_startTag + 3;
    
    //The received string must be in the range of [5, 8].
    //If it's ouside this range it returns.
    if((receive_endTag-receive_startTag)<5 || (receive_endTag-receive_startTag)>8){
        return;
    }
    
    value = receive_getNext(value);
    numberAcknowledged = receive_parseInt();



    //Remove acknowledged number.
    //storage_removeHit returns the hitNumber it removed.
    //If it returns zero then it means nothing was removed.
    //This would mean that the hit number was not stored.
    
    //Returns the number it removed, if we ever wanna do something with that.
    numberAcknowledged = storage_removeHit(numberAcknowledged);

    //Then responds to bluetooth saying it removed it.
    response_hitAcknowledged(numberAcknowledged);
    
    /*
    //Testing:
    char myString[100] = {' '};
    sprintf(myString, "\n %d number acknowledged!", numberAcknowledged);
    bluetooth_printString(myString);
    gpio_toggle_bit(GPIOB, 1);
    */

}

void received_resetBuffer(void){
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

void receive_processString(void){
    char receive_messageType = ' ';

    if(receive_buffer[receive_startTag]!='#'){
        received_resetBuffer();
        return;
    } 

    if(receive_buffer[receive_endTag]!='#'){
        received_resetBuffer();
        return; 
    }
    
    //If it does not have a comma after the message type, it returns.
    if(receive_buffer[receive_startTag+2]!=','){
        return;
    }
    
    //If it does not have a comma before the final #, it returns.
    if(receive_buffer[receive_endTag-1]!=','){
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
        case 'C':
            receive_connected();
            break;
        default:
            break;
    }

    received_resetBuffer();

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
            received_resetBuffer();
            break;
        }

        receive_position++;

    }

    //startTag cannot be zero. endTag cannot be zero.
    //endTag must be bigger than startTag.
    if(receive_startTag!=0 && receive_endTag!=0 && (receive_endTag - receive_startTag)>0 && receive_endTag>receive_startTag){
        receive_processString();
    }
    
}


// ********* RECEIVE ENDS HERE **********************************


void communication_start(void){
    bluetooth_start();
    exti_attach_interrupt(BLUETOOTH_AFIO_EXTI_PIN, BLUETOOTH_AFIO_EXTI_PORT, received_bluetooth, EXTI_RISING);

}
