


#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"

#define BIT(N)  (1U<<(N))	//mascara de bits

#define USE_AHB


void xdelay(unsigned volatile v) {  //função delay

    while( --v ) {}
}



void
GPIO_Init(uint32_t outputs, uint32_t inputs) {
GPIOA_Type *gpio;

#ifdef USE_AHB
    gpio = GPIOF_AHB;
    /* Enable AHB access for Port F  */
    SYSCTL->GPIOHBCTL |= BIT(5);
#else
    gpio = GPIOF;
#endif

     /* Enable clock for Port F         */
     SYSCTL->RCGCGPIO  |= BIT(5);

     /* To modify PF0, it is necessary to unlock it. See 10.1 of Datasheet */
     if( (inputs|outputs)&1 ) {
         gpio->LOCK = 0x4C4F434B;                // unlock to set PF0
         *(uint32_t * )(&(gpio->CR))   = 0x0;    // CR is marked read-only
     }

    // /* Pins for led are digital output */
    gpio->DIR    = outputs;         /* Only specified bits are outputs */
    gpio->DEN    = inputs|outputs;  /* All pins are digital I/O        */
    gpio->PUR    = inputs;  //ativa resistor de pullup das portas de entrada
    xdelay(10);
     if( (inputs|outputs)&1 ) {
         gpio->LOCK = 0x0;                       // lock again
         *(uint32_t * )(&(gpio->CR))   = 0x0;    // CR is marked read-only
    }

}



uint32_t GPIO_ReadPins(void){   //retorna valores lidos na porta F

GPIOA_Type *gpio;

#ifdef USE_AHB
    gpio = GPIOF_AHB;
#else
    gpio = GPIOF;
#endif
    return gpio->DATA;  //valores nos switchs sao '0' apertados
    xdelay(10);
}



void GPIO_WritePins(uint32_t zeroes, uint32_t ones){    //escreve na porta F

GPIOA_Type *gpio;

#ifdef USE_AHB
    gpio = GPIOF_AHB;
    SYSCTL->GPIOHBCTL |= BIT(5);
#else
    gpio = GPIOF;
#endif
    gpio->DATA = (gpio->DATA&~zeroes)|ones;

} 



int main(void) {	//função main **** Lembrar de inicializar portas ****

    


return 0;
}


