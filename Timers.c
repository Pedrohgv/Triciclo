#include "Timers.h"

#define USE_AHB

TIMER0_Type * GetTimerAddress(uint32_t timer_module){

    switch (timer_module)  //selects timer
    {
    case TIMER_0:
    return TIMER0;
    break;

    case TIMER_1:
    return TIMER1;
    break;

    case TIMER_2:
    return TIMER2;
    break;

    case TIMER_3:
    return TIMER3;
    break;

    case TIMER_4:
    return TIMER4;
    break;

    case TIMER_5:
    return TIMER5;
    break;
	}
}

void ConfigureTimer (uint32_t timer_module, uint32_t timer_mode, uint32_t interrupt, uint32_t match_value)	//configures timer
{

	TIMER0_Type *timer;
	timer = GetTimerAddress(timer_module); 	//get timer address
	
    SYSCTL->RCGCTIMER |= BIT(timer_module); //enables clock for timer

	timer->CTL &= 0xFFFFFFFE; 	//disables timer for configuration
	timer->CFG = 0x00000000;   	//selects concatenated mode
	timer->TAMR |= BIT(4) | timer_mode;	// selects 'count up' mode, and selects timer mode
	timer->TAILR = match_value;	//loads the match value for timer
	timer->IMR |= interrupt;	//configures interrupt (time-out mode)
    xdelay(10);

}

void EnableTimer (uint32_t timer_module)
{
	TIMER0_Type *timer;
	timer = GetTimerAddress(timer_module);	//get timer address

	timer->CTL |= 0x00000001;	//enables timer
}

void DisableTimer (uint32_t timer_module)
{
	TIMER0_Type *timer;
	timer = GetTimerAddress(timer_module);	//get timer address
	
	timer->CTL &= 0xFFFFFFFE;	//disables timer
}

void ClearTimerInterruptStatus (uint32_t timer_module)   //clear the interrupt status so program can continue
{
    TIMER0_Type *timer;
    timer = GetTimerAddress(timer_module);   //get timer address

    timer->ICR |= 0x00000001;   
}