#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"
#include "GPIO.h"

#define ADC_0  BIT(0)   //define ADC modules
#define ADC_1  BIT(1)

#define AIN0   0x0      //define ADC pins
#define AIN1   0x1
#define AIN2   0x2
#define AIN3   0x3
#define AIN4   0x4
#define AIN5   0x5
#define AIN6   0x6
#define AIN7   0x7
#define AIN8   0x8
#define AIN9   0x9
#define AIN10  0xA
#define AIN11  0xB

void GPIO_ADC(uint32_t port, uint32_t analog, uint32_t ADC_module, uint32_t ADC_pin);    //enable and configures ADC function (using sample 0 sequencer 0)
uint32_t Get_ADC_Conversion(uint32_t ADC_module); //return value from ADC conversion on ADC module specified by adc

#ifdef __cplusplus
}
#endif

#endif