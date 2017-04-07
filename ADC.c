#include "ADC.h"

void GPIO_ADC(uint32_t port, uint32_t analog, uint32_t ADC_module, uint32_t ADC_pin){     //enable and configures ADC function (using sample 0 sequencer 0)
    GPIOA_Type *gpio;
    ADC0_Type  *adc;    //ADC converter

    switch (ADC_module) { //select ADC module
        case ADC_0:
        adc = ADC0;
        break;
        case ADC_1:
        adc = ADC1;
        break;
    }
    
    gpio = Get_Port_Adress(port);  

    SYSCTL->RCGCADC |= ADC_module;    //enable clock for ADC0    

    SYSCTL->RCGCGPIO |= port;   //enable clock for port
    
    gpio->AFSEL |= analog;  //enable pin to be controlled by an associated peripheral  (!!!!!!!!!!!!!!)
    gpio->DEN &= !analog;   //disable digital function
    gpio->AMSEL |= analog;  //disable analog isolation

    adc->ACTSS &= !0x00000001;  //disable sample sequencer 0
    adc->EMUX &= 0xFFFFFFF0;    //select processor signal as event trigger for sampling 
    adc->SSMUX0 |= ADC_pin;    //select AIN0 (PE3) as input for sample 0 of sample sequencer 0
    adc->SSCTL0 |= 0x00000002;     //set sample 0 of sequencer 0 to be the last sample
    adc->PC   |= 0x00000001;      //set sampling rate as 125 ksps
    adc->ACTSS |= 0x00000001;   //enable sample sequencer 0
   
}

uint32_t Get_ADC_Conversion(uint32_t ADC_module) { //return value from ADC conversion on ADC module specified by adc

    ADC0_Type  *adc;    //ADC converter

    switch (ADC_module) { //select ADC module
        case ADC_0:
        adc = ADC0;
        break;
        case ADC_1:
        adc = ADC1;
        break;
    }

    adc->PSSI |= 0x00000001; //iniciates sampling process for sample sequencer 0  
    xdelay(10);
    adc->PSSI = 0;
    
    return (adc->SSFIFO0 & 0x00000FFF);  //return conversion result masked for 12 bits

}


