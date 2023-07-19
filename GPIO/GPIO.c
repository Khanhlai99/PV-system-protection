/*
 * GPIO.c
 * Description: 4 LEDs to indicate where fault happen
 *              2 Switches for user interface
 *              SSR connection pin:     PV      PH0
 *                                      Boost   PH1
 *                                      Load    PL2
 *                                      BiDC    PK6
 *                                      Batt    PL3
 */

#include "../include.h"
#include "GPIO.h"

//***************************************************************************************************************************
//                                                       VARIABLES DEFINE
//                                                   global variables in GPIO.c
//***************************************************************************************************************************
static void SW_ISR(void);

/* CONFIGURATION GPIO */
extern void Config_LED(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(PORTF, PIN0 | PIN4);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinTypeGPIOOutput(PORTN, PIN0 | PIN1);

    GPIOPinWrite(PORTN, PIN0 | PIN1, 0x00);
    GPIOPinWrite(PORTF, PIN0 | PIN4, 0x00);                                 // LEDs are initially off
}

extern void Config_SW(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    GPIOPinTypeGPIOInput(PORTJ, SW_1 | SW_2);
    GPIOPadConfigSet(PORTJ, SW_1 | SW_2 , GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);     //Switch using 8mA with pull-up resistor

    /*Switch interrupt config*/
    GPIOIntRegister(PORTJ, &SW_ISR);
    GPIOIntEnable(PORTJ, SW1_Int);
    GPIOIntEnable(PORTJ, SW2_Int);

    IntEnable(INT_GPIOJ);
}

extern void Config_GPIO(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);

    GPIOPinTypeGPIOOutput(PORTH, PIN0 | PIN1);
    GPIOPinTypeGPIOOutput(PORTL, PIN2 | PIN3);
    GPIOPinTypeGPIOOutput(PORTK, PIN6);

    GPIOPinWrite(PORTH, PIN0 | PIN1, 0x00);
    GPIOPinWrite(PORTL, PIN2 | PIN3, 0x00);
    GPIOPinWrite(PORTK, PIN6, 0x00);
}
/*
 * GPIO FUNCTION *
 */
extern void LED_control(int led_id, bool status) //LED control
{
    switch (led_id){
    /* led_id = 1, 2, 3, 4 corresponding to D1, D2, D3, D4
     * led_id = 0 means all LEDs
     */
    case 1:
        if (status == on){
            GPIOPinWrite(PORTN, PIN1, PIN1);
        }
//        else
//            GPIOPinWrite(PORTN, PIN1, 0x00);
        break;
    case 2:
        if (status == on){
            GPIOPinWrite(PORTN, PIN0, PIN0);
        }
        break;
    case 3:
        if (status == on){
            GPIOPinWrite(PORTF, PIN4, PIN4);
        }
        break;
    case 4:
        if (status == on){
            GPIOPinWrite(PORTF, PIN0, PIN0);
        }
        break;

    case 0:
        if (status == on){                          //turn on all LEDs
            GPIOPinWrite(PORTN, PIN0 | PIN1, 0x03);
            GPIOPinWrite(PORTF, PIN0 | PIN4, 0x11);
        }
        else{                                       //turn off all LEDs
            GPIOPinWrite(PORTN, PIN0 | PIN1, 0x00);
            GPIOPinWrite(PORTF, PIN0 | PIN4, 0x00);
        }

        break;
    }
}

//void ssr_all(bool status){
//    if (status == on){                                              //turn on all SSR
//        GPIOPinWrite(PORTH, PIN0 | PIN1, 0x03);
//        GPIOPinWrite(PORTL, PIN2 | PIN3, 0x0C);
//        GPIOPinWrite(PORTK, PIN6, 0x40);
//    }
//    else{                                                           //turn off all SSR
//        GPIOPinWrite(PORTH, PIN0 | PIN1, 0x00);
//        GPIOPinWrite(PORTL, PIN2 | PIN3, 0x00);
//        GPIOPinWrite(PORTK, PIN6, 0x00);
//    }
//}

extern void ssr_control(uint8_t ssr_id, bool status){
    if (status == off){
        switch (ssr_id){
        case 0:
            GPIOPinWrite(PORTH, PIN0, off);
            break;
        case 1:
            GPIOPinWrite(PORTH, PIN1, off);
            break;
        case 2:
            GPIOPinWrite(PORTL, PIN2, off);
            break;
        case 3:
            GPIOPinWrite(PORTK, PIN6, off);
            break;
        case 4:
            GPIOPinWrite(PORTL, PIN3, off);
            break;
        case 5:
            GPIOPinWrite(PORTH, PIN0 | PIN1, 0x00);
            GPIOPinWrite(PORTL, PIN2 | PIN3, 0x00);
            GPIOPinWrite(PORTK, PIN6, 0x00);
        }
    }
    else if (status == on){
        switch (ssr_id){
        case 0:
            GPIOPinWrite(PORTH, PIN0, on);
            break;
        case 1:
            GPIOPinWrite(PORTH, PIN1, on);
            break;
        case 2:
            GPIOPinWrite(PORTL, PIN2, on);
            break;
        case 3:
            GPIOPinWrite(PORTK, PIN6, on);
            break;
        case 4:
            GPIOPinWrite(PORTL, PIN3, on);
            break;
        case 5:
           GPIOPinWrite(PORTH, PIN0 | PIN1, 0x03);
           GPIOPinWrite(PORTL, PIN2 | PIN3, 0x0C);
           GPIOPinWrite(PORTK, PIN6, 0x40);
        }
    }
}
//***************************************************************************************************************************
//                                                          GPIO INTERRUPT
//***************************************************************************************************************************
static void SW_ISR(void)
{
    if(system_status == 0 && GPIOIntStatus(PORTJ, true) == SW_1)
    {
        system_status = 1;
//        ssr_control(5, on);
        start_ADC();
    }
    else if(system_status == 1 && GPIOIntStatus(PORTJ, true) == SW_2)
    {
        system_status = 0;
//        ssr_control(5, off);
    }
    GPIOIntClear(PORTJ, GPIOIntStatus(PORTJ, true));
}
