/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Bryan Newbold.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#ifndef _SPEAKER_H_
#define _SPEAKER_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>
#include <libmaple/timer.h>

typedef timer_mode TimerMode;


void speaker_pause(void);
void speaker_resume(void);
void speaker_setPrescaleFactor(uint32);
void speaker_setOverflow(uint16);
uint16 speaker_getOverflow(void);
void speaker_setCount(uint16);
uint16 speaker_setPeriod(uint32);
void speaker_setMode(int, timer_mode mode);
void speaker_setCompare(int, uint16);
void speaker_attachInterrupt(int, voidFuncPtr);
void speaker_detachInterrupt(int);
void speaker_refresh(void);
void speaker_start(void);
void speaker_toggle(void);
void speaker_shoot(void);
void speaker_hit(void);
void speaker_playHit(void);
void speaker_playShoot(void);

#ifdef __cplusplus
}
#endif

#endif 