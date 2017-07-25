#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"

#define BIT(N)  (1U<<(N))	//bitmask

#define PORT_A BIT(0)   //define ports
#define PORT_B BIT(1)
#define PORT_C BIT(2)
#define PORT_D BIT(3)
#define PORT_E BIT(4)
#define PORT_F BIT(5)

#define YES		0x0001
#define NO 		0x0000
#define	ON 		0x0001	
#define OFF 	0x0000

#define INPUT 	0
#define OUTPUT 	1

#define FALLING_EDGE 0
#define RISING_EDGE	 1

#define PULL_DOWN_RESISTOR	1
#define PULL_UP_RESISTOR	2

typedef struct {	//struct for pin designation
	uint32_t port;
	uint32_t pin;
}i_o;

void xdelay(uint32_t count);    //simple delay function
GPIOA_Type * GetPortAddress(uint32_t port);	//returns port address 
void GPIOInit(i_o io, uint32_t direction, uint32_t internal_input_resistor);	//initialize digital GPIO pins
uint32_t GPIOReadPin(i_o io);   //return value from input pin
void GPIOWritePin(i_o io, uint32_t value);    //writes on specific pin

void GPIOInterruptInit(i_o io, uint32_t event); //configures a edge-detect interrupt
void GPIOInterruptEnable(i_o io);
void GPIOInterruptDisable(i_o io);
void GPIOClearInterruptStatus(i_o io);


#ifdef __cplusplus
}
#endif

#endif