


#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"



#define USE_AHB

uint16_t test_variable = 0;





#include "Triciclo.h"

void IoInit(void)   //pin initialization
{
    GPIOInit(RED_LED, OUTPUT);
    GPIOInit(BLUE_LED, OUTPUT);
    GPIOInit(GREEN_LED, OUTPUT);
    GPIOInit(CHARGE_MODE_PIN, INPUT);
    GPIOInit(DRIVE_MODE_PIN, INPUT);
    GPIOInit(CHARGE_PIN,OUTPUT);
    GPIOInit(MOTOR_PIN,OUTPUT);
}

void IntInit(void)  //initialize interrupts
{
    NVIC_EnableIRQ(ADC0SS0_IRQn);   //enables interrupt for sample sequencer 0 of ADC module 0
    NVIC_EnableIRQ(ADC0SS1_IRQn);   //enables interrupt for sample sequencer 1 of ADC module 0
    NVIC_EnableIRQ(TIMER0A_IRQn);   //enables interrupt for for timer 0
    NVIC_EnableIRQ(TIMER1A_IRQn);   //enables interrupt for for timer 1
    NVIC_EnableIRQ(TIMER2A_IRQn);   //enables interrupt for for timer 2

    __enable_irq(); //enables interrupt block

}

void TIMER16_0A_IRQHandler(void)
{
    
    ClearTimerInterruptStatus(TIMER_0);
    StartCurrentRead;
}

void ADC0Seq0_IRQHandler(void)
{
    ClearADCInterruptStatus (ADC_0,SS_0);    //clear the interrupt status so program can continue
    

    test_variable = !test_variable;


    if(test_variable == 1)
    {
        TurnOnMotor;
        TurnOnWhiteLed;
    } 
    else
    {
        TurnOffMotor;
        TurnOnRedLed;
    }
   // while(1){}
    

}

int main(void) {	//função main **** Lembrar de inicializar portas ****

IoInit();        //port initialization
IntInit();      //interrupt initialization

ConfigureADC(VOLTAGE_READ_PIN, ADC_0, AIN9, SS_0, PROCESSOR, ADC_INTERRUPT_ENABLE);
EnableSampleSequencer (ADC_0, SS_0);

ConfigureTimer(TIMER_0, PERIODIC_MODE, TIMER_INTERRUPT_ENABLE, FIVE_SEC);   //

EnableTimer(TIMER_0);

while(1){

    if(ChargeModePin == ON)   //charge mode
    {
        /* faz um monte de coisa


        
        */
        TurnOnBlueLed;
        while(ChargeModePin == ON){}
    }   //end of charge mode

    
    if(DriveModePin == ON)     //drive mode
    {
        TurnOnWhiteLed;
        

        while(DriveModePin == ON)
        {     

        }   
    }   //end of drive mode
}   //end of primary loop

return 0;
}


