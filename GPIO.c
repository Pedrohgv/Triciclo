
#include "GPIO.h"

#define USE_AHB

void xdelay(uint32_t count){    //simple delay function
    while(count > 0)
        count--;
}

GPIOA_Type * GetPortAdress(uint32_t port) {       //return adress port

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


void GPIOInit(i_o io, uint32_t direction) {	//initialize digital GPIO pins
GPIOA_Type *gpio;

gpio = GetPortAdress(io.port); 

     /* Enable clock for Port         */
     SYSCTL->RCGCGPIO  |= io.port;

     gpio->DEN |= BIT(io.pin);  //set pin as digital i/o
    
     if(direction == OUTPUT)
        gpio->DIR |= BIT(io.pin);  //set pin as output or input
     else
        gpio->PDR |= BIT(io.pin);  //enable pull down resistor

    //  /* To modify PF0, it is necessary to unlock it. See 10.1 of Datasheet */
    //  if( ((inputs|outputs)&1) && port==PORT_F) {
    //      gpio->LOCK = 0x4C4F434B;                // unlock to set PF0
    //      *(uint32_t * )(&(gpio->CR))   = 0x0;    // CR is marked read-only
    //  }

    // // /* Pins for led are digital output */
    // gpio->DIR    = outputs;         /* Only specified bits are outputs */
    // gpio->DEN    = outputs | inputs;  /* Only specified bits are digital        */
    // gpio->PDR    = inputs;  //ativa resistor de pullup das portas de entrada
    xdelay(10);
     // if( (inputs|outputs)&1 && port==PORT_F ) {
     //     gpio->LOCK = 0x0;                       // lock again
     //     *(uint32_t * )(&(gpio->CR))   = 0x0;    // CR is marked read-only
    // }

}

uint32_t GPIOReadPin(i_o io){   //return value from input pin

GPIOA_Type *gpio;

gpio = GetPortAdress(io.port);

    return ((gpio->DATA & BIT(io.pin)) >> io.pin);  
    xdelay(10);
}



void GPIOWritePin(i_o io, uint32_t value){    //writes on specific pin

GPIOA_Type *gpio;

gpio = GetPortAdress(io.port);
#ifdef USE_AHB
    SYSCTL->GPIOHBCTL |= io.port;
#endif
    gpio->DATA = (gpio->DATA & ~BIT(io.pin)) | (value << io.pin);

} 

