/*
************************
    Storage library
    This library is in charge of storing hit information.
    You initialize it with storage_start();
    Then when you get hit you 
*/

#include "storage.h"
#include <string.h>


//The number assigned with a hit.
int numberOfHits = 1;

//Array of shots.
//All are initialized to zero. 
hit shot_List[MAXIMUM_NUMBER_HITS] = {0, NULL_TERMINATED_GPS_STRING, 0};

//The position we are in on the array. 
//This gets looped to zero once it reaches MAXIMUM_NUMBER_HITS-1. 
int shot_listPosition = 0;

//
int storage_unsentEntries(void){
    int counter=0;
    int i=0;
    for(i=0; i<MAXIMUM_NUMBER_HITS; i++){
        if(shot_List[i].ID!=0){
            counter++;
        }
    }
    return counter;

}

//Checks the whole array for any shots.
//If any is found then it's returned. Otherwise it returns element zero. 
hit storage_getShot(void){
    int i = MAXIMUM_NUMBER_HITS - 1;
    while(i>=0){
        if(shot_List[i].ID!=0){
            return shot_List[i];
        }
        i--;
    }
    return shot_List[0];
}

void storage_resetEntry(int position){
    shot_List[position].ID = 0;
    strcpy(shot_List[position].location, NULL_TERMINATED_GPS_STRING);
    shot_List[position].hitNumber = 0;

}

//Returns the number it removed.
//If nothing is removed it returns zero. 
int storage_removeHit(int number){
    int i=0;
    for(i=0; i<MAXIMUM_NUMBER_HITS; i++){
        if(shot_List[i].hitNumber==number){
            storage_resetEntry(i);
            return number;
        }
    }
    return 0;

}

void storage_increaseList(void){
    if(shot_listPosition==(MAXIMUM_NUMBER_HITS-1)){
        shot_listPosition=0;
    } else { 
        shot_listPosition++;
    }
}

void storage_increaseNumberHits(void){
    numberOfHits++;
}

void storage_start(void){

    numberOfHits = 1;

    int i;
    for(i=0; i < MAXIMUM_NUMBER_HITS; i++){
        shot_List[i].ID = 0;
        strcpy(shot_List[i].location, NULL_TERMINATED_GPS_STRING);
        shot_List[i].hitNumber = 0;
    }

}

void storage_reset(void){
    storage_reset();
}

void storage_add(int id, char* gps){
    shot_List[shot_listPosition].ID = id;
    strcpy(shot_List[shot_listPosition].location, gps);
    shot_List[shot_listPosition].hitNumber = numberOfHits;
    storage_increaseNumberHits();
    storage_increaseList();
}












