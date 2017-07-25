#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"
#include "GPIO.h"

#define ADC_0	BIT(0)   //define ADC modules
#define ADC_1	BIT(1)

#define	SS_0	0	//define sample sequencers
#define	SS_1	1
#define	SS_2	2
#define	SS_3	3

#define AIN0   0x0      //PE3         //define ADC pins
#define AIN1   0x1		//PE2
#define AIN2   0x2		//PE1
#define AIN3   0x3		//PE0
#define AIN4   0x4		//PD3
#define AIN5   0x5		//PD2
#define AIN6   0x6		//PD1
#define AIN7   0x7		//PD0
#define AIN8   0x8		//PE5
#define AIN9   0x9		//PE4
#define AIN10  0xA		//PB4	
#define AIN11  0xB		//PB5

#define	PROCESSOR			0x0		//trigger sources for ADC conversion
#define	ANALOG_COMPARATOR_0	0x1
#define ANALOG_COMPARATOR_1	0x2
#define	EXTERNAL			0x4
#define	TIMER				0x5
#define	PWM_GENERATOR_0		0x6
#define	PWM_GENERATOR_1		0x7
#define	PWM_GENERATOR_2		0x8
#define	PWM_GENERATOR_3		0x9
#define	AlWAYS				0xF

#define	ADC_INTERRUPT_ENABLE		0x1
#define ADC_INTERUPT_DISABLE		0x0

void ConfigureADC(i_o io, uint32_t ADC_module, uint32_t ADC_pin, uint32_t sample_sequencer, uint32_t trigger_source, uint32_t interrupt);     //enable and configures ADC function (using sample 0)
void EnableSampleSequencer (uint32_t ADC_module, uint32_t sample_sequencer);
void StartADCConversion (uint32_t ADC_module, uint32_t sample_sequencer);
void ClearADCInterruptStatus (uint32_t ADC_module, uint32_t sample_sequencer);    //clear the interrupt status so program can continue
uint32_t GetADCConversion (uint32_t ADC_module, uint32_t sample_sequencer); //return value from ADC conversion from specified ADC module and sample sequencer

#ifdef __cplusplus
}
#endif

#endif