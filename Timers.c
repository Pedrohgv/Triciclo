#include "Timers.h"

#define USE_AHB

TIMER0_Type * GetTimerAdress(uint8_t timer_module){

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

void ConfigureTimer (uint8_t timer_module, uint8_t timer_mode, uint8_t interrupt, uint32_t match_value)	//configures timer
{

	TIMER0_Type *timer;
	timer = GetTimerAdress(timer_module); 	//get timer adress
	
    SYSCTL->RCGCTIMER |= BIT(timer_module); //enabels clock for timer

	timer->CTL = 0xFFFFFFFE; 	//disables timer for configuration
	timer->CFG = 0x00000000;   	//selects concatenated mode
	timer->TAMR |= BIT(4) | timer_mode;	// selects 'count up' mode, and selects timer mode
	timer->TAILR = match_value;	//loads the match value for timer
	timer->IMR |= interrupt;	//configures interrrupt (time-out mode)

}

void EnableTimer (uint8_t timer_module)
{
	TIMER0_Type *timer;
	timer = GetTimerAdress(timer_module);	//get timer adress

	timer->CTL |= 0x00000001;	//enables timer
}

void DisableTimer (uint8_t timer_module)
{
	TIMER0_Type *timer;
	timer = GetTimerAdress(timer_module);	//get timer adress
	
	timer->CTL &= 0xFFFFFFFE;	//disables timer
}

void ClearTimerInterruptStatus (uint8_t timer_module)   //clear the interupt status so program can continue
{
    TIMER0_Type *timer;
    timer = GetTimerAdress(timer_module);   //get timer adress

    timer->ICR |= 0x00000001;   
}