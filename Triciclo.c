


#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"



#define USE_AHB

#include "Triciclo.h"

uint32_t test_variable = 0;
uint32_t flag_mode = CHARGE_MODE;
uint32_t flag_LED_state = FLAG_LED_RED;    //flag for LED state, used in blink for 

void IoInit(void)   //pin initialization
{
    GPIOInit(RED_LED, OUTPUT, 0);
    GPIOInit(BLUE_LED, OUTPUT, 0);
    GPIOInit(GREEN_LED, OUTPUT, 0);
    GPIOInterruptInit(CHARGE_MODE_PIN, FALLING_EDGE);
    GPIOInterruptInit(DRIVE_MODE_PIN, FALLING_EDGE);
    GPIOInterruptInit(START_MOTOR_PIN, RISING_EDGE);
    GPIOInit(CHARGE_PIN,OUTPUT, 0);
    GPIOInit(MOTOR_PIN,OUTPUT, 0);
    TurnOffMotor;
    TurnOffCharge;
}

void TimersInit(void)
{
    ConfigureTimer(TIMER_0, PERIODIC_MODE, TIMER_INTERRUPT_ENABLE, ONE_SEC);    //timer for panel and battery voltage read
    ConfigureTimer(TIMER_1, ONE_SHOT_MODE, TIMER_INTERRUPT_ENABLE, HALF_SEC);    //timer for stoppung the motor    
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
    NVIC_EnableIRQ(GPIOF_IRQn);   //enables interrupt for port f pins
    NVIC_EnableIRQ(GPIOA_IRQn);   //enables interrupt for port f pins

    GPIOInterruptEnable(CHARGE_MODE_PIN);   //enables interrupt for charge mode button
    GPIOInterruptEnable(DRIVE_MODE_PIN);    //enables interrupt for drive mode button

    __enable_irq(); //enables interrupt block

}

void TIMER16_0A_IRQHandler(void)
{
    ClearTimerInterruptStatus(TIMER_0);
    if(flag_mode == CHARGE_MODE)
    {
        StartPanelVoltageRead;
    }
    else if(flag_mode == DRIVE_MODE)
    {
        StartBatteryVoltageRead;
    }
    GPIOInterruptEnable(CHARGE_MODE_PIN);   //re-enable external interrupts for changing operation mode
    GPIOInterruptEnable(DRIVE_MODE_PIN);
}

void TIMER16_1A_IRQHandler(void)
{
    ClearTimerInterruptStatus(TIMER_1);
    TurnOffMotor;
    EnablePeriodicVoltageRead;
}

void ADC0Seq0_IRQHandler(void)              //ADC voltage read from panel
{
    ClearADCInterruptStatus (ADC_0,SS_0);    //clear the interrupt status so program can continue

    uint32_t ADC = PanelVoltageRead;

    char char_float[10];
    Ftoa(ADC, char_float, 5);
    PrintString(UART_0, char_float);
    PrintChar(UART_0, '\n');

    if(ADC > VOLTAGE_PANEL_LOW)
    {
        TurnOnCharge;
        TurnOnBlueLed;
    }
    else
    {
        TurnOffCharge;
        if(flag_LED_state == FLAG_LED_RED)
        {
            TurnOnRedLed;
            flag_LED_state = FLAG_LED_BLUE;
        }
        else if(flag_LED_state == FLAG_LED_BLUE)
        {
            TurnOnBlueLed;
            flag_LED_state = FLAG_LED_RED;
        }
    }
    
}

void ADC0Seq1_IRQHandler(void)
{
    ClearADCInterruptStatus (ADC_0,SS_1);    //clear the interrupt status so program can continue

    uint32_t ADC = BatteryVoltageRead;

    char char_float[10];
    Ftoa(ADC, char_float, 5);
    PrintString(UART_0, char_float);
    PrintChar(UART_0, '\n');

    if(ADC > VOLTAGE_BATTERY_EMPTY)
    {
        EnableMotorStart;
        TurnOnGreenLed;
    }
    else
    {
        DisableMotorStart;
        if(flag_LED_state == FLAG_LED_RED)
        {
            TurnOnRedLed;
            flag_LED_state = FLAG_LED_GREEN;
        }
        else if(flag_LED_state == FLAG_LED_GREEN)
        {
            TurnOnGreenLed;
            flag_LED_state = FLAG_LED_RED;
        }
    }
  
    
}

void GPIOF_IRQHandler(void)
{
    GPIOInterruptDisable(CHARGE_MODE_PIN);
    GPIOInterruptDisable(DRIVE_MODE_PIN);

    DisablePeriodicVoltageRead; //disable periodic timer for avoiding the external interrupt to be re-enabled too fast

    flag_LED_state = FLAG_LED_RED;  //resets flag LED to red for 1st blink in case of low voltage
    
    if(ChargeModePin == 0)  //charge mode pin is ON? (pin is low-level activated)
    {
        TurnOffMotor;
        flag_mode = CHARGE_MODE;
    }
    else if(DriveModePin == 0)  //drive mode pin is ON? (pin is low-level activated)
    {
        TurnOffCharge;
        flag_mode = DRIVE_MODE;
    }
    GPIOClearInterruptStatus(CHARGE_MODE_PIN);
    GPIOClearInterruptStatus(DRIVE_MODE_PIN);

    EnablePeriodicVoltageRead;  //re-enable periodic timer
}

void GPIOA_IRQHandler(void) //interrupt handler for motor activation
{
  GPIOInterruptDisable(START_MOTOR_PIN);

  DisablePeriodicVoltageRead; //disable periodic timer for avoiding the external interrupt to be re-enabled too fast

  if(StartMotorPin == ON)
  {
    TurnOnMotor;
  }

  GPIOClearInterruptStatus(START_MOTOR_PIN);
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

flag_mode = CHARGE_MODE;
EnablePeriodicVoltageRead;

while(1);

return 0;
}   //end of main