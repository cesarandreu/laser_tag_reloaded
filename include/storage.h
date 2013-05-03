
#ifndef _STORAGE_H_
#define _STORAGE_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#define MAXIMUM_NUMBER_HITS 256
#define MAXIMUM_GPS_LENGTH 25
#define NULL_TERMINATED_GPS_STRING "                        "

//Defines a struct for hit. 
typedef struct hit {
    int ID; //ID of the player that hit you.
    char location[MAXIMUM_GPS_LENGTH]; // GPS String of location where you got hit.
    int hitNumber; //Number asociated with a shot
} hit;

int storage_unsentEntries(void);
hit storage_getShot(void);
//void storage_resetEntry(int);
int storage_removeHit(int);
//void storage_increaseList(void);
//void storage_increaseNumberHits(void);
void storage_start(void);
void storage_reset(void);
void storage_add(int, char*);

#ifdef __cplusplus
}
#endif

#endif 
