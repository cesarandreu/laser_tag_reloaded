#ifndef _RECEIVER_H_
#define _RECEIVER_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>

int listenForIR(void);
void listenSignal(void);
uint8 interpretCode(void);
void receiver_start(void);

#ifdef __cplusplus
}
#endif

#endif 