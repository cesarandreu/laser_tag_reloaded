#ifndef _PLAYER_H_
#define _PLAYER_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_PLAYER_CODE 1
#define DEFAULT_SHOTS_FIRED 0

// [ 2 <-> (2^8)-1] are the allowed numbers (between 2 and 255)
#define MAXIMUM_PLAYER_NUMBER 255
#define MINIMUM_PLAYER_NUMBER 2
    
    
void player_start(int);
void player_shoot(void);
int player_getShots(void);
int player_getCode(void);
void player_setCode(int);
void player_reset(void);

#ifdef __cplusplus
}
#endif

#endif 
