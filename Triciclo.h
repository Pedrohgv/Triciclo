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
extern uint32_t flag_motor;
extern uint32_t flag_voltage_level;

const i_o RX_PIN		  =	 {.port = PORT_A, .pin = 0};	//pins for uart receiver/transmitter
const i_o TX_PIN		  =	 {.port = PORT_A, .pin = 1};

const i_o CHARGE_MODE_PIN =  {.port = PORT_A, .pin = 7};    //input pin for 'charge mode'
const i_o DRIVE_MODE_PIN  =  {.port = PORT_A, .pin = 6};   //input pin for 'charge mode'
const i_o START_MOTOR_PIN =  {.port = PORT_A, .pin = 5};	//input pin for starting the motor

const i_o CHARGE_PIN 	  =  {.port = PORT_B, .pin = 0};	//output pin for enabling charge
const i_o MOTOR_PIN		  =	 {.port = PORT_B, .pin = 1};	//output pin for motor activation

const i_o RED_LED         =  {.port = PORT_F, .pin = 1};    //red led
const i_o BLUE_LED        =  {.port = PORT_F, .pin = 2};    //blue led
const i_o GREEN_LED       =  {.port = PORT_F, .pin = 3};    //green led 

const i_o PANEL_VOLTAGE_READ_PIN = {.port = PORT_E, .pin = 4};	//Analog input for panel voltage read (ADC pin PE4)
const i_o BATTERY_VOLTAGE_READ_PIN = {.port = PORT_E, .pin = 5};	//Analog input for battery voltage read (ADC pin PE5)

#define	OK	1		//flags for voltage level
#define LOW 0

#define	ONE_SEC			20000000	//time for periodic current and voltage reads
#define	FIVE_SEC		100000000	//motor activation time

//*********** ADC/Volts ratio = 246,805218 ADC/Volt

 #define	VOLTAGE_BATTERY_EMPTY	2715	//ADC read, equals 11 volts at battery
 #define	VOLTAGE_PANEL_LOW		3085	//ADC read, equals 12,5 volts at panel

// #define VOLTAGE_BATTERY_EMPTY 1927		//for testing with 9V DC source
// #define	VOLTAGE_PANEL_LOW     1927

#define	ChargeModePin   GPIOReadPin(CHARGE_MODE_PIN)			//reading of 'charging mode' pin
#define	DriveModePin    GPIOReadPin(DRIVE_MODE_PIN)			//reading of "drive mode" pin
#define	StartMotorPin	GPIOReadPin(START_MOTOR_PIN)

#define TurnOffRedLed   GPIOWritePin(RED_LED, OFF)	//macros for turning led's off
#define TurnOffBlueLed  GPIOWritePin(BLUE_LED, OFF)
#define TurnOffGreenLed GPIOWritePin(GREEN_LED, OFF)
#define TurnOffAllLeds  TurnOffRedLed; TurnOffBlueLed; TurnOffGreenLed

#define TurnOnRedLed 	TurnOffAllLeds; GPIOWritePin(RED_LED, ON)	//macros for  turning led's on
#define TurnOnBlueLed 	TurnOffAllLeds; GPIOWritePin(BLUE_LED, ON)
#define TurnOnGreenLed 	TurnOffAllLeds; GPIOWritePin(GREEN_LED, ON)
#define TurnOnWhiteLed  GPIOWritePin(RED_LED, ON); GPIOWritePin(BLUE_LED, ON); GPIOWritePin(GREEN_LED, ON)

#define TurnOnCharge    GPIOWritePin(CHARGE_PIN,ON)
#define TurnOnMotor     GPIOWritePin(MOTOR_PIN,ON); EnableTimer(TIMER_2); flag_motor = ON

#define TurnOffCharge   GPIOWritePin(CHARGE_PIN,OFF)
#define TurnOffMotor    GPIOWritePin(MOTOR_PIN,OFF); flag_motor = OFF

#define	StartPanelVoltageRead		StartADCConversion(ADC_0, SS_0)
#define	StartBatteryVoltageRead	StartADCConversion(ADC_0, SS_1)

#define	PanelVoltageRead	GetADCConversion(ADC_0, SS_0)	
#define	BatteryVoltageRead	GetADCConversion(ADC_0, SS_1)

#define EnablePeriodicPanelVoltageRead 		EnableTimer(TIMER_0)
#define	DisablePeriodicPanelVoltageRead		DisableTimer(TIMER_0)

#define EnablePeriodicBatteryVoltageRead 		EnableTimer(TIMER_1)
#define	DisablePeriodicBatteryVoltageRead		DisableTimer(TIMER_1)

void IoInit(void);   //pin initialization
void IntInit(void);  //initialize interrupts
void UARTInit(void); //initialize UART


#ifdef __cplusplus
}
#endif

#endif