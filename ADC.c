#include "ADC.h"

#define USE_AHB

void ConfigureADC(i_o io, uint32_t ADC_module, uint32_t ADC_pin, uint32_t sample_sequencer, uint32_t trigger_source, uint32_t interrupt)     //enable and configures ADC function (using sample 0)
{
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
    
    gpio = GetPortAddress(io.port);  

    SYSCTL->RCGCADC |= ADC_module;    //enable clock for ADC   

    SYSCTL->RCGCGPIO |= io.port;   //enable clock for port
    
    gpio->AFSEL |= BIT(io.pin);  //allows pin to be controlled by an associated peripheral  
    gpio->DEN &= ~BIT(io.pin);   //disable digital function
    gpio->AMSEL |= BIT(io.pin);  //disable analog isolation

    adc->ACTSS &= ~(BIT(sample_sequencer));  //disable sample sequencer 
    switch (sample_sequencer)
    {
        case SS_0:
        adc->EMUX = (adc->EMUX & 0xFFFFFFF0) | trigger_source;    //select trigger source for sequencer 0
        adc->SSMUX0 |= ADC_pin;    //select Pin as input for sample 0 of sample sequencer 0
        adc->SSCTL0 |= (0x2 | (interrupt << 2)); //select sample 0 as end of sequence and configure interrupt
        break;

        case SS_1:
        adc->EMUX = (adc->EMUX & 0xFFFFFF0F) | (trigger_source << 4);    //select trigger source for sequencer 1
        adc->SSMUX1 |= ADC_pin;    //select Pin as input for sample 0 of sample sequencer 1
        adc->SSCTL1 |= (0x2 | (interrupt << 2)); //select sample 0 as end of sequence and configure interrupt
        break;

        case SS_2:
        adc->EMUX = (adc->EMUX & 0xFFFFF0FF) | (trigger_source << 8);    //select trigger source for sequencer 2
        adc->SSMUX2 |= ADC_pin;    //select Pin as input for sample 0 of sample sequencer 2
        adc->SSCTL2 |= (0x2 | (interrupt << 2)); //select sample 0 as end of sequence and configure interrupt
        break;

        case SS_3:
        adc->EMUX = (adc->EMUX & 0xFFFF0FFF) | (trigger_source << 12);    //select trigger source for sequencer 3
        adc->SSMUX3 |= ADC_pin;    //select Pin as input for sample 0 of sample sequencer 3
        adc->SSCTL3 |= (0x2 | (interrupt << 2)); //select sample 0 as end of sequence and configure interrupt
        break;
    }

    adc->IM   |=  (interrupt << sample_sequencer);  //configure interrupt mask
    adc->PC   |= 0x00000001;      //set sampling rate as 125 ksps

    /* SAMPLE SEQUENCER MUST BE ENABLED   */
   
}

void EnableSampleSequencer (uint32_t ADC_module, uint32_t sample_sequencer)   
{
    ADC0_Type  *adc;    //ADC converter

    switch (ADC_module) { //select ADC module
        case ADC_0:
        adc = ADC0;
        break;
        case ADC_1:
        adc = ADC1;
        break;
    }

    adc->ACTSS  |=  BIT(sample_sequencer);
}

void StartADCConversion (uint32_t ADC_module, uint32_t sample_sequencer)   
{
    ADC0_Type  *adc;    //ADC converter

    switch (ADC_module) { //select ADC module
        case ADC_0:
        adc = ADC0;
        break;
        case ADC_1:
        adc = ADC1;
        break;
    }
    adc->PSSI |= BIT(sample_sequencer); 
}

void ClearADCInterruptStatus (uint32_t ADC_module, uint32_t sample_sequencer)    //clear the interrupt status so program can continue
{
    ADC0_Type  *adc;    //ADC converter

    switch (ADC_module) { //select ADC module
        case ADC_0:
        adc = ADC0;
        break;
        case ADC_1:
        adc = ADC1;
        break;
    }

    adc->ISC |= BIT(sample_sequencer);
}

uint32_t GetADCConversion (uint32_t ADC_module, uint32_t sample_sequencer) //return value from ADC conversion from specified ADC module and sample sequencer
{
    ADC0_Type  *adc;    //ADC converter

    switch (ADC_module) { //select ADC module
        case ADC_0:
        adc = ADC0;
        break;
        case ADC_1:
        adc = ADC1;
        break;
    }
    
    switch (sample_sequencer)
    {
        case SS_0:
        return (adc->SSFIFO0 & 0x0FFF);  //return conversion result masked for 12 bits of sample sequencer 0
        break;

        case SS_1:
        return (adc->SSFIFO1 & 0x0FFF);  //return conversion result masked for 12 bits of sample sequencer 1
        break;
        
        case SS_2:
        return (adc->SSFIFO2 & 0x0FFF);  //return conversion result masked for 12 bits of sample sequencer 2
        break;
        
        case SS_3:
        return (adc->SSFIFO3 & 0x0FFF);  //return conversion result masked for 12 bits of sample sequencer 3
        break;
    }
}


