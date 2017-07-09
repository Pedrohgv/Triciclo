


#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"



#define USE_AHB

#include "Triciclo.h"

uint32_t test_variable = 0;
uint32_t flag_motor = OFF;
uint32_t flag_voltage_level = LOW;

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
    ConfigureTimer(TIMER_0, PERIODIC_MODE, TIMER_INTERRUPT_ENABLE, ONE_SEC);    //timer for panel voltage read
    ConfigureTimer(TIMER_1, PERIODIC_MODE, TIMER_INTERRUPT_ENABLE, ONE_SEC);    //timer for battery voltage read
    ConfigureTimer(TIMER_2, ONE_SHOT_MODE, TIMER_INTERRUPT_ENABLE, ONE_SEC);   //timer for stopping motor
}

void ADCInit(void)
{
    ConfigureADC(PANEL_VOLTAGE_READ_PIN, ADC_0, AIN9, SS_0, PROCESSOR, ADC_INTERRUPT_ENABLE); //configure ADC read for panel voltage
    ConfigureADC(BATTERY_VOLTAGE_READ_PIN, ADC_0, AIN8, SS_1, PROCESSOR, ADC_INTERRUPT_ENABLE); //configure ADC read for battery voltage
    EnableSampleSequencer(ADC_0, SS_0); //enables ADC read for panel voltage
    EnableSampleSequencer(ADC_0, SS_1); //enables ADC read for battery voltage

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
    StartPanelVoltageRead; 
}

void TIMER16_1A_IRQHandler(void)
{
    
    ClearTimerInterruptStatus(TIMER_1);
    StartBatteryVoltageRead;
}

void TIMER16_2A_IRQHandler(void)
{
    
    ClearTimerInterruptStatus(TIMER_2);
    TurnOffMotor;

}

void ADC0Seq0_IRQHandler(void)
{
    ClearADCInterruptStatus (ADC_0,SS_0);    //clear the interrupt status so program can continue

    uint32_t ADC = PanelVoltageRead;
    float voltage = (3.3 / 4095) * ADC;
    char char_float[10];
    Ftoa(voltage, char_float, 5);
    PrintString(UART_0, char_float);
    PrintChar(UART_0, '\n');
    TurnOnGreenLed;
    
}

void ADC0Seq1_IRQHandler(void)
{
    ClearADCInterruptStatus (ADC_0,SS_1);    //clear the interrupt status so program can continue
    
    if(BatteryVoltageRead > VOLTAGE_BATTERY_EMPTY)
    {
        flag_voltage_level = OK;
        TurnOnGreenLed;
    }
    else
    {
        flag_voltage_level = LOW;
        TurnOffRedLed;
    }
  
    
}

void UARTInit(void)
{
    ConfigureUART(UART_0, RX_PIN, TX_PIN);
}

int main(void) {	//função main **** Lembrar de inicializar portas ****

SystemInit();
IoInit();        //port initialization
TimersInit();   //timers initialization
ADCInit();      //ADC modules initialization
IntInit();      //interrupt initialization
UARTInit();

EnablePeriodicPanelVoltageRead;

while(1);
// while(1){   //main loop

//     if(ChargeModePin == ON)   //charge mode
//     {
//         TurnOffMotor;
//         EnablePeriodicPanelVoltageRead;

//         while(ChargeModePin == ON){}    

//         DisablePeriodicPanelVoltageRead;
//         TurnOffCharge; 
//     }   //end of charge mode

    
//     if(DriveModePin == ON)     //drive mode
//     {
//         TurnOffCharge;
//         EnablePeriodicBatteryVoltageRead;

//         while(DriveModePin == ON)
//         {
//             if((StartMotorPin == ON) && (flag_voltage_level == OK))
//             {
//                 TurnOnMotor;
//             }
//             while(flag_motor == ON){}
//         }
//         DisablePeriodicBatteryVoltageRead;
//     }   //end of drive mode

// }   //end of primary loop

return 0;
}   //end of main


