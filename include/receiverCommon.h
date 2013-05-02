#ifndef _RECEIVERCOMMON_H_
#define _RECEIVERCOMMON_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#define ZERO_DURATION 541
#define MAXPULSE 8125 // This value is 65000/8 (arbitrarily selected 8).
#define RESOLUTION 20
#define WANTED_PULSES 5

#ifdef __cplusplus
}
#endif

#endif 
