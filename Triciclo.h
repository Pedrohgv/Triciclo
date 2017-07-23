#ifndef TRICICLO_H
#define TRICICLO_H

#ifdef __cplusplus
extern "C" {
#endif


#include 	"TM4C123GH6PM.h"
#include 	"system_TM4C123.h"
#include	"GPIO.h"
#include	"ADC.h"
#include	"Timers.h"
#include	"UART.h"

extern uint32_t test_variable;
extern uint32_t flag_mode;

const i_o RX_PIN		  =	 {.port = PORT_A, .pin = 0};	//pins for uart receiver/transmitter
const i_o TX_PIN		  =	 {.port = PORT_A, .pin = 1};

const i_o CHARGE_MODE_PIN =  {.port = PORT_F, .pin = 4};    //input pin for 'charge mode'
const i_o DRIVE_MODE_PIN  =  {.port = PORT_F, .pin = 0};   //input pin for 'charge mode'
const i_o START_MOTOR_PIN =  {.port = PORT_A, .pin = 2};	//input pin for starting the motor

const i_o CHARGE_PIN 	  =  {.port = PORT_B, .pin = 0};	//output pin for enabling charge
const i_o MOTOR_PIN		  =	 {.port = PORT_B, .pin = 1};	//output pin for motor activation

const i_o RED_LED         =  {.port = PORT_F, .pin = 1};    //red led
const i_o BLUE_LED        =  {.port = PORT_F, .pin = 2};    //blue led
const i_o GREEN_LED       =  {.port = PORT_F, .pin = 3};    //green led 

const i_o PANEL_VOLTAGE_READ_PIN = {.port = PORT_E, .pin = 4};	//Analog input for panel voltage read (ADC pin PE4)
const i_o BATTERY_VOLTAGE_READ_PIN = {.port = PORT_E, .pin = 5};	//Analog input for battery voltage read (ADC pin PE5)



#define	OK	1		//flags for voltage level
#define LOW 0

#define CHARGE_MODE 	0
#define DRIVE_MODE 		1

#define FLAG_LED_RED    0
#define FLAG_LED_GREEN  1
#define FLAG_LED_BLUE   1

#define HALF_SEC		10000000
#define	ONE_SEC			20000000	//time for periodic current and voltage reads
#define	FIVE_SEC		100000000	

//*********** ADC/Volts ratio = 246,805218 ADC/Volt

#define	VOLTAGE_BATTERY_EMPTY	2962	//ADC read, equals 12 volts at battery 
#define	VOLTAGE_PANEL_LOW		3549	//ADC read, equals 14,4 volts at panel (13,5 volts at battery)

// #define VOLTAGE_BATTERY_EMPTY 1927		//for testing with 9V DC source
// #define	VOLTAGE_PANEL_LOW     1927

#define	ChargeModePin   GPIOReadPin(CHARGE_MODE_PIN)			//reading of 'charging mode' pin
#define	DriveModePin    GPIOReadPin(DRIVE_MODE_PIN)			//reading of "drive mode" pin
#define	StartMotorPin	GPIOReadPin(START_MOTOR_PIN)

#define TurnOffRedLed   GPIOWritePin(RED_LED, OFF)	//macros for turning LED's off
#define TurnOffBlueLed  GPIOWritePin(BLUE_LED, OFF)
#define TurnOffGreenLed GPIOWritePin(GREEN_LED, OFF)
#define TurnOffAllLeds  TurnOffRedLed; TurnOffBlueLed; TurnOffGreenLed

#define TurnOnRedLed 	TurnOffAllLeds; GPIOWritePin(RED_LED, ON)	//macros for  turning LED's on
#define TurnOnBlueLed 	TurnOffAllLeds; GPIOWritePin(BLUE_LED, ON)
#define TurnOnGreenLed 	TurnOffAllLeds; GPIOWritePin(GREEN_LED, ON)
#define TurnOnWhiteLed  GPIOWritePin(RED_LED, ON); GPIOWritePin(BLUE_LED, ON); GPIOWritePin(GREEN_LED, ON)

#define TurnOnCharge    GPIOWritePin(CHARGE_PIN,ON)
#define TurnOnMotor     GPIOWritePin(MOTOR_PIN,ON); EnableTimer(TIMER_1)

#define TurnOffCharge   GPIOWritePin(CHARGE_PIN,OFF)
#define TurnOffMotor    GPIOWritePin(MOTOR_PIN,OFF)

#define	StartPanelVoltageRead	StartADCConversion(ADC_0, SS_0)
#define	StartBatteryVoltageRead	StartADCConversion(ADC_0, SS_1)

#define	PanelVoltageRead	GetADCConversion(ADC_0, SS_0)	
#define	BatteryVoltageRead	GetADCConversion(ADC_0, SS_1)

#define EnablePeriodicVoltageRead 		EnableTimer(TIMER_0)
#define DisablePeriodicVoltageRead 		DisableTimer(TIMER_0)

#define	EnableMotorStart				GPIOInterruptEnable(START_MOTOR_PIN)
#define	DisableMotorStart				GPIOInterruptDisable(START_MOTOR_PIN)



void IoInit(void);   //pin initialization
void TimersInit(void); //timers initialization
void ADCInit(void);    //ADC blocks initialization
void IntInit(void);  //initialize interrupts
void UARTInit(void); //initialize UART
void PutInSleepMode(void); 	//puts in sleep mode


#ifdef __cplusplus
}
#endif

#endif