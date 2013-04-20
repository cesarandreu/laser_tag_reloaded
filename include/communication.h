#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "bluetooth.h"
#include "storage.h"
#include "enemy.h"
#include "player.h"

void communication_start(void);


#ifdef __cplusplus
}
#endif

#endif 