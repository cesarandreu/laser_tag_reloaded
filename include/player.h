#ifndef _PLAYER_H_
#define _PLAYER_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_PLAYER_CODE 1
#define DEFAULT_SHOTS_FIRED 0

void player_start(int);
void player_shoot(void);
unsigned int player_getShots(void);
int player_getCode(void);
void player_setCode(int);
void player_reset(void);

#ifdef __cplusplus
}
#endif

#endif 





