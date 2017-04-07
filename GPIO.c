
#include GPIO.h

GPIOA_Type * Get_Port_Adress(uint32_t port) {       //return adress port

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


void GPIO_Init(uint32_t port, uint32_t outputs, uint32_t inputs) {	//initialize digital GPIO pins
GPIOA_Type *gpio;

gpio = Get_Port_Adress(port); 

     /* Enable clock for Port         */
     SYSCTL->RCGCGPIO  |= port;

     /* To modify PF0, it is necessary to unlock it. See 10.1 of Datasheet */
     if( ((inputs|outputs)&1) && port==PORT_F) {
         gpio->LOCK = 0x4C4F434B;                // unlock to set PF0
         *(uint32_t * )(&(gpio->CR))   = 0x0;    // CR is marked read-only
     }

    // /* Pins for led are digital output */
    gpio->DIR    = outputs;         /* Only specified bits are outputs */
    gpio->DEN    = outputs | inputs;  /* Only specified bits are digital        */
    gpio->PUR    = inputs;  //ativa resistor de pullup das portas de entrada
    xdelay(10);
     if( (inputs|outputs)&1 && port==PORT_F ) {
         gpio->LOCK = 0x0;                       // lock again
         *(uint32_t * )(&(gpio->CR))   = 0x0;    // CR is marked read-only
    }

}

uint32_t GPIO_ReadPins(uint32_t port){   //return values from specific GPIO port

GPIOA_Type *gpio;

gpio = Get_Port_Adress(port);

    return gpio->DATA;  
    xdelay(10);
}



void GPIO_WritePins(uint32_t port, uint32_t zeroes, uint32_t ones){    //escreve na porta F

GPIOA_Type *gpio;

gpio = Get_Port_Adress(port);
#ifdef USE_AHB
    SYSCTL->GPIOHBCTL |= port;
#endif
    gpio->DATA = (gpio->DATA&~zeroes)|ones;

}