#ifndef _ENEMY_H_
#define _ENEMY_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#define MAXIMUM_NUMBER_ENEMIES 16

int enemy_checkExist(int);
void enemy_addEnemy(int);
void enemy_reset(void);
void enemy_start(void);

#ifdef __cplusplus
}
#endif

#endif 
