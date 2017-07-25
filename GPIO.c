
#include "GPIO.h"

#define USE_AHB

void xdelay(uint32_t count){    //simple delay function
    while(count > 0)
        count--;
}

GPIOA_Type * GetPortAddress(uint32_t port) {       //return address port

#ifdef USE_AHB
    SYSCTL->GPIOHBCTL |= port;  //Enable AHB access for Port
    switch (port){
        
        case PORT_A:
        return GPIOA_AHB;
        break;
        
        case PORT_B:
        return GPIOB_AHB;
        break;
       
        case PORT_C:
        return GPIOC_AHB;
        break;

        case PORT_D:
        return GPIOD_AHB;
        break;

        case PORT_E:
        return GPIOE_AHB;
        break;

        case PORT_F:
        return GPIOF_AHB;
        break;
    }
    #else
    switch (port){
        
        case PORT_A:
        return GPIOA;
        break;
        
        case PORT_B:
        return GPIOB;
        break;
       
        case PORT_C:
        return GPIOC;
        break;

        case PORT_D:
        return GPIOD;
        break;

        case PORT_E:
        return GPIOE;
        break;

        case PORT_F:
        return GPIOF;
        break;
    }
#endif
}


void GPIOInit(i_o io, uint32_t direction, uint32_t internal_input_resistor) {	//initialize digital GPIO pins
    GPIOA_Type *gpio;

    gpio = GetPortAddress(io.port); 

    /* Enable clock for Port         */
    SYSCTL->RCGCGPIO  |= io.port;

    //  /* To modify PF0, it is necessary to unlock it. See 10.1 of Datasheet */
    if( (BIT(io.pin) & 1) && (io.port==PORT_F))
    {
        gpio->LOCK = 0x4C4F434B;                // unlock to set PF0
        *(uint32_t * )(&(gpio->CR)) = 0x1;    // CR is marked read-only
    }

    gpio->DEN |= BIT(io.pin);  //set pin as digital i/o

    if(direction == OUTPUT)
       gpio->DIR |= BIT(io.pin);  //set pin as output
    else if(internal_input_resistor == PULL_DOWN_RESISTOR)
       gpio->PDR |= BIT(io.pin);  //enable pull down resistor
    else if(internal_input_resistor == PULL_UP_RESISTOR)
       gpio->PUR |= BIT(io.pin); //enable pull up resistor

    if((BIT(io.pin) & 1) && (io.port==PORT_F))
    {
        gpio->LOCK = 0x0;                       // lock again
        *(uint32_t * )(&(gpio->CR))   = 0x0;    // CR is marked read-only
    }
    xdelay(10);
}

uint32_t GPIOReadPin(i_o io){   //return value from input pin

GPIOA_Type *gpio;

gpio = GetPortAddress(io.port);

    return ((gpio->DATA & BIT(io.pin)) >> io.pin);  
    xdelay(10);
}



void GPIOWritePin(i_o io, uint32_t value){    //writes on specific pin

GPIOA_Type *gpio;

gpio = GetPortAddress(io.port);
#ifdef USE_AHB
    SYSCTL->GPIOHBCTL |= io.port;
#endif
    gpio->DATA = (gpio->DATA & ~BIT(io.pin)) | (value << io.pin);

} 

void GPIOInterruptInit(i_o io, uint32_t event) //configures a edge-detect interrupt
{
    GPIOA_Type *gpio;
    gpio = GetPortAddress(io.port); //get port address

    
    if(event == RISING_EDGE)    //set detection event as rising or falling edge
    {
        GPIOInit(io, INPUT, PULL_DOWN_RESISTOR);  //initialize pin with pull down resistor
        gpio->IEV |= BIT(io.pin);   //set detection event as rising edge
    }    
    else
    {
        GPIOInit(io, INPUT, PULL_UP_RESISTOR);  //initialize pin with pull up resistor
    }
    xdelay(10);
}

void GPIOInterruptEnable(i_o io)
{
    GPIOA_Type *gpio;
    gpio = GetPortAddress(io.port); //get port address

    gpio->IM |= BIT(io.pin);    //enables interrupt 
}

void GPIOInterruptDisable(i_o io)
{
    GPIOA_Type *gpio;
    gpio = GetPortAddress(io.port); //get port address

    gpio->IM &= ~BIT(io.pin);    //disables interrupt
}

void GPIOClearInterruptStatus(i_o io)
{
    GPIOA_Type *gpio;
    gpio = GetPortAddress(io.port); //get port address

    gpio->ICR |= BIT(io.pin);    //clear interrupt status
}
