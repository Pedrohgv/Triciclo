#include "UART.h"

#define USE_AHB

UART0_Type * GetUARTAddress (uint32_t uart_module)	//returns address of UART module
{
	switch (uart_module)
	{
		case UART_0:
		return UART0;
		break;

		case UART_1:
		return UART1;
		break;

		case UART_2:
		return UART2;
		break;

		case UART_3:
		return UART3;
		break;

		case UART_4:
		return UART4;
		break;

		case UART_5:
		return UART5;
		break;

		case UART_6:
		return UART6;
		break;

		case UART_7:
		return UART7;
		break;

	}
}

void ConfigureUART (uint32_t uart_module, i_o rx_pin, i_o tx_pin)	//configures UART with baud rate = 9600 bps, 1 stop bit, no parity bits
{
	GPIOA_Type *gpio_rx;
	GPIOA_Type *gpio_tx;	
	UART0_Type *uart;	//uart address

	gpio_rx = GetPortAddress(rx_pin.port);
	gpio_tx = GetPortAddress(tx_pin.port);
	uart = GetUARTAddress(uart_module);

	SYSCTL->RCGCGPIO |= (rx_pin.port | tx_pin.port);	//enables clock for rx and tx pins
	SYSCTL->RCGCUART |= uart_module;		//enables clock for uart module

	gpio_rx->AFSEL |= BIT(rx_pin.pin);	//allows pin to be controlled by an associated peripheral
	gpio_tx->AFSEL |= BIT(tx_pin.pin);
	
	gpio_rx->PCTL &= ~(0x0000000F << (rx_pin.pin << 2)); 	//erases current value of PCTL register for peripheral control assignment 
	gpio_tx->PCTL &= ~(0x0000000F << (tx_pin.pin << 2));
	gpio_rx->PCTL |= (1 << (rx_pin.pin << 2));	//assign UART to control rx and tx pins
	gpio_tx->PCTL |= (1 << (tx_pin.pin << 2));

	

	gpio_rx->DEN |= BIT(rx_pin.pin);	//enables digital function for rx and tx pins
	gpio_tx->DEN |= BIT(tx_pin.pin);

	uart->CTL &= ~0x00000001;	//disables uart module for safe configuration

	uart->IBRD = 130;	//configure baud rate as 9600 bps
	uart->FBRD = 13;

	uart->LCRH = (3<<5);	//8 bits word, 1 stop bit, no parity bit
	uart->CC = 0x0; 		//system clock selected as clock source

	uart->CTL |= (BIT(9) | BIT(8) | BIT(0));	//enables transmitting, receiving and the entire module
	xdelay(10);	//waits for stabilization
	
}

void PrintChar (uint32_t uart_module, char c)	
{
	UART0_Type *uart;	//uart address
	uart = GetUARTAddress(uart_module);

	while((uart->FR & (1<<5)) != 0){}	//check if any data is still being transmitting
	uart->DR = c;
}

char ReadChar (uint32_t uart_module)
{
	UART0_Type *uart;	//uart address
	uart = GetUARTAddress(uart_module);

	while((uart->FR & (1<<4)) != 0);		//check if register has data

	return (uart->DR & 0x0FF);
}

void PrintString (uint32_t uart_module, char * string)
{
	UART0_Type *uart;	//uart address
	uart = GetUARTAddress(uart_module);

	while(*string)
		PrintChar(uart_module, *(string++));
}


/////////////// Functions for converting float to string//////////////////////////////////////////

// reverses a string 'str' of length 'len'
void Reverse(char *str, uint32_t len)
{
    uint32_t i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
 
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int IntToStr(uint32_t x, char str[], uint32_t d)
{
	uint32_t i = 0;

	if(x == 0)	//if integer is 0
	{
		str[0] = '0';
		str[1] = '\0';
		return 1;
	}   
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    Reverse(str, i);
    str[i] = '\0';
    return i;
}
 
// Converts a floating point number to string.
void Ftoa(float n, char *res, uint32_t afterpoint)
{
	uint32_t count = 0;
    // Extract integer part
    uint32_t ipart = (uint32_t)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 	
    // convert integer part to string
    uint32_t i = IntToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        for(count = 0; count < afterpoint; count++)
        {
        	fpart = fpart * 10;
        }
        IntToStr((uint32_t)fpart, res + i + 1, afterpoint);
    }
}