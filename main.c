

/**
 * main.c
 * Protection System v2.0
 * Hardware: TM4C129EXL
 */


#include "include.h"

//GLOBAL VARIABLES
int position = 0;

bool fault_flag = 0;         // 0 ~ no fault, 1 ~ fault happens

int index = 0;

void main(void)
{
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
    Config_LED();
    Config_SW();
    Config_GPIO();
    Config_ADC();


    IntMasterEnable();

    ssr_control(5, on);
    start_ADC();

    while(1)
    {

    }
}
