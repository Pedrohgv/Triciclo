#ifndef TIMERS_H
#define TIMERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"
#include "GPIO.h"

#define TIMER_0 0   //timers and counters
#define TIMER_1 1
#define TIMER_2 2
#define TIMER_3 3
#define TIMER_4 4
#define TIMER_5 5
#define COUNTER_A 0
#define COUNTER_B 1

#define ONE_SHOT_MODE	1	//modes for timers
#define	PERIODIC_MODE	2
#define	CAPTURE_MODE	3

#define	TIMER_INTERRUPT_DISABLE	0	//interrupt flags
#define	TIMER_INTERRUPT_ENABLE	1

void ConfigureTimer (uint32_t timer_module, uint32_t timer_mode, uint32_t interrupt, uint32_t match_value);	//configures timer
void EnableTimer(uint32_t timer_module);
void DisableTimer (uint32_t timer_module);
void ClearTimerInterruptStatus (uint32_t timer_module);   //clear the interrupt status so program can continue

#ifdef __cplusplus
}
#endif

#endif