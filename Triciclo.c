


#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"



#define USE_AHB

#include "Triciclo.h"

uint16_t test_variable = 0;
uint8_t flag_motor = OFF;
uint8_t flag_voltage_level = LOW;

void IoInit(void)   //pin initialization
{
    GPIOInit(RED_LED, OUTPUT);
    GPIOInit(BLUE_LED, OUTPUT);
    GPIOInit(GREEN_LED, OUTPUT);
    GPIOInit(CHARGE_MODE_PIN, INPUT);
    GPIOInit(DRIVE_MODE_PIN, INPUT);
    GPIOInit(START_MOTOR_PIN, INPUT);
    GPIOInit(CHARGE_PIN,OUTPUT);
    GPIOInit(MOTOR_PIN,OUTPUT);
    TurnOffMotor;
    TurnOffCharge;
}

void TimersInit(void)
{
    ConfigureTimer(TIMER_0, PERIODIC_MODE, TIMER_INTERRUPT_ENABLE, ONE_SEC);    //timer for current read
    ConfigureTimer(TIMER_1, PERIODIC_MODE, TIMER_INTERRUPT_ENABLE, ONE_SEC);    //timer for voltage read
    ConfigureTimer(TIMER_2, ONE_SHOT_MODE, TIMER_INTERRUPT_ENABLE, FIVE_SEC);   //timer for stopping motor
}

void ADCInit(void)
{
    ConfigureADC(CURRENT_READ_PIN, ADC_0, AIN8, SS_0, PROCESSOR, ADC_INTERRUPT_ENABLE); //configure ADC read for current
    ConfigureADC(VOLTAGE_READ_PIN, ADC_0, AIN9, SS_1, PROCESSOR, ADC_INTERRUPT_ENABLE); //configure ADC read for voltage
    EnableSampleSequencer(ADC_0, SS_0); //enables ADC read for current
    EnableSampleSequencer(ADC_0, SS_1); //enables ADC read for voltage

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

void TIMER16_1A_IRQHandler(void)
{
    
    ClearTimerInterruptStatus(TIMER_1);
    StartVoltageRead;
}

void TIMER16_2A_IRQHandler(void)
{
    
    ClearTimerInterruptStatus(TIMER_2);
    TurnOffMotor;

}

void ADC0Seq0_IRQHandler(void)
{
    ClearADCInterruptStatus (ADC_0,SS_0);    //clear the interrupt status so program can continue

    if(CurrentRead <= CURRENT_BATTERY_FULL)
    {
        TurnOffCharge;
        TurnOnWhiteLed;
    }
    else
    {
        TurnOnCharge;
        TurnOnBlueLed;
    }
}

void ADC0Seq1_IRQHandler(void)
{
    ClearADCInterruptStatus (ADC_0,SS_1);    //clear the interrupt status so program can continue
    
    if(DriveModePin == ON)  //voltage read in drive mode, for checking if battery has enough power
    {
        if(VoltageRead > VOLTAGE_BATTERY_EMPTY)
        {
            flag_voltage_level = OK;
            TurnOnGreenLed;
        } 
        else
        {
            flag_voltage_level = LOW;
            TurnOnRedLed;
        }
    }
    else    //voltage read in charge mode, to check if battery isn't full
    {
        if(VoltageRead < VOLTAGE_BATTERY_FULL)
        {
            TurnOnBlueLed;
            TurnOnCharge;
            EnablePeriodicCurrentRead;
        }
        else
        {
            TurnOnWhiteLed;
        }
    }
    
}

int main(void) {	//função main **** Lembrar de inicializar portas ****

IoInit();        //port initialization
TimersInit();   //timers initialization
ADCInit();      //ADC modules initialization
IntInit();      //interrupt initialization


while(1){   //main loop

    if(ChargeModePin == ON)   //charge mode
    {
        StartVoltageRead;

        while(ChargeModePin == ON){}    

        DisablePeriodicCurrentRead;
        TurnOffCharge; 
    }   //end of charge mode

    
    if(DriveModePin == ON)     //drive mode
    {
        EnablePeriodicVoltageRead;

        while(DriveModePin == ON)
        {
            if((StartMotorPin == ON) && (flag_voltage_level == OK))
            {
                TurnOnMotor;
            }
            while(flag_motor == ON){}
        }
    }   //end of drive mode

}   //end of primary loop

return 0;
}   //end of main


