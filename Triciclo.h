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
const i_o RED_LED         =  {.port = PORT_F, .pin = 1};    //red led
const i_o BLUE_LED        =  {.port = PORT_F, .pin = 2};    //blue led
const i_o GREEN_LED       =  {.port = PORT_F, .pin = 3};    //greeb led 

#define	ChargeModePin   GPIOReadPin(CHARGE_MODE_PIN)			//reading of 'charging mode' pin
#define	DriveModePin    GPIOReadPin(DRIVE_MODE_PIN)			//reading of "drive mode" pin

#define TurnOnRedLed 	GPIOWritePin(RED_LED, ON)	//macros for  turning led's on
#define TurnOnBlueLed 	GPIOWritePin(BLUE_LED, ON)
#define TurnOnGreenLed 	GPIOWritePin(GREEN_LED, ON)

#define TurnOffRedLed   GPIOWritePin(RED_LED, OFF)	//macros for turning led's off
#define TurnOffBlueLed  GPIOWritePin(BLUE_LED, OFF)
#define TurnOffGreenLed GPIOWritePin(GREEN_LED, OFF)

void IoInit(void);   //pin initialization


#ifdef __cplusplus
}
#endif

#endif