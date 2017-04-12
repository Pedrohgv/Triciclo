


#include "TM4C123GH6PM.h"
#include "system_TM4C123.h"











#include "Triciclo.h"

void IoInit(void)   //pin initialization
{
    GPIOInit(RED_LED, OUTPUT);
    GPIOInit(BLUE_LED, OUTPUT);
    GPIOInit(GREEN_LED, OUTPUT);
    GPIOInit(CHARGE_MODE_PIN, INPUT);
    GPIOInit(DRIVE_MODE_PIN, INPUT);
}

int main(void) {	//função main **** Lembrar de inicializar portas ****

//#define USE_AHB

IoInit();        //port initialization
//TurnOnBlueLed;
//while(1){}
while(1){

    if(ChargeModePin == ON)   //charge mode
    {
        /* faz um monte de coisa


        
        */
        TurnOnRedLed;
        while(ChargeModePin == ON){}

        TurnOffRedLed;
    }   //end of charge mode


    if(DriveModePin == ON)
        TurnOnBlueLed;

    while(DriveModePin == ON)     //drive mode
    {

    }   //end of drive mode
    TurnOffBlueLed;
}   //end of primary loop

return 0;
}


