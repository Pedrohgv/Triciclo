
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

GPIOA_Type * Get_Port_Adress(uint32_t port);	//returns port adress 
void GPIO_Init(uint32_t port, uint32_t outputs, uint32_t inputs);	//initialize digital GPIO pins
uint32_t GPIO_ReadPins(uint32_t port);   //return values from specific GPIO port
void GPIO_WritePins(uint32_t port, uint32_t zeroes, uint32_t ones);    //escreve na porta F


#ifdef __cplusplus
}
#endif

#endif