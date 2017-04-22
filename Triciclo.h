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

const i_o CHARGE_MODE_PIN =  {.port = PORT_A, .pin = 7};    //input pin for 'charge mode'
const i_o DRIVE_MODE_PIN  =  {.port = PORT_A, .pin = 6};   //input pin for 'charge mode'
const i_o CHARGE_PIN 	  =  {.port = PORT_B, .pin = 1};	//output pin for enabling charge
const i_o MOTOR_PIN		  =	 {.port = PORT_B, .pin = 2};	//output pin for motor activation
const i_o RED_LED         =  {.port = PORT_F, .pin = 1};    //red led
const i_o BLUE_LED        =  {.port = PORT_F, .pin = 2};    //blue led
const i_o GREEN_LED       =  {.port = PORT_F, .pin = 3};    //greeb led 

const i_o VOLTAGE_READ_PIN = {.port = PORT_E, .pin = 4};	//Analog input for voltage read (ADC pin PE4)
const i_o CURRENT_READ_PIN = {.port = PORT_E, .pin = 5};	//Analog input for current read (ADC pin PE5)

#define		ONE_SEC			0x05F5E100	//time for periodic current and voltage reads
#define		FIVE_SEC		0x1DCD6500	//motor activation time


#define	ChargeModePin   GPIOReadPin(CHARGE_MODE_PIN)			//reading of 'charging mode' pin
#define	DriveModePin    GPIOReadPin(DRIVE_MODE_PIN)			//reading of "drive mode" pin

#define TurnOffRedLed   GPIOWritePin(RED_LED, OFF)	//macros for turning led's off
#define TurnOffBlueLed  GPIOWritePin(BLUE_LED, OFF)
#define TurnOffGreenLed GPIOWritePin(GREEN_LED, OFF)
#define TurnOffAllLeds  TurnOffRedLed; TurnOffBlueLed; TurnOffGreenLed

#define TurnOnRedLed 	TurnOffAllLeds; GPIOWritePin(RED_LED, ON)	//macros for  turning led's on
#define TurnOnBlueLed 	TurnOffAllLeds; GPIOWritePin(BLUE_LED, ON)
#define TurnOnGreenLed 	TurnOffAllLeds; GPIOWritePin(GREEN_LED, ON)
#define TurnOnWhiteLed  GPIOWritePin(RED_LED, ON); GPIOWritePin(BLUE_LED, ON); GPIOWritePin(GREEN_LED, ON)

#define TurnOnCharge    GPIOWritePin(CHARGE_PIN,ON)
#define TurnOnMotor     GPIOWritePin(MOTOR_PIN,ON)

#define TurnOffCharge   GPIOWritePin(CHARGE_PIN,OFF)
#define TurnOffMotor    GPIOWritePin(MOTOR_PIN,OFF)

#define	StartCurrentRead	StartADCConversion(ADC_0, SS_0)
#define	StartVoltageRead	StartADCConversion(ADC_0, SS_1)

#define	GetCurrentRead	GetADCConversion(ADC_0, SS_0)	
#define	GetVoltageRead	GetADCConversion(ADC_1, SS_0)

void IoInit(void);   //pin initialization
void IntInit(void);  //initialize interrupts


#ifdef __cplusplus
}
#endif

#endif