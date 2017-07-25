#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"
#include "GPIO.h"

#define UART_0	BIT(0)
#define UART_1	BIT(1)
#define UART_2	BIT(2)
#define UART_3	BIT(3)
#define UART_4 	BIT(4)
#define UART_5	BIT(5)
#define UART_6	BIT(6)
#define UART_7	BIT(7)

UART0_Type * GetUARTAddress (uint32_t uart_module);					//returns address of UART module
void ConfigureUART (uint32_t uart_module, i_o rx_pin, i_o tx_pin);	//configures UART with baud rate = 9600 bps, 1 stop bit, no parity bits
void PrintChar (uint32_t uart_module, char c);
char ReadChar (uint32_t uart_module);
void PrintString (uint32_t uart_module, char * string);

/////////////// Functions for converting float to string//////////////////////////////////////////
void Reverse(char *str, uint32_t len);							//reverses string 
int IntToStr(uint32_t x, char str[], uint32_t d);				//converts integer into string
void Ftoa(float n, char *res, uint32_t afterpoint);				//converts float into string

#ifdef __cplusplus
}
#endif

#endif