/*
 * Timer.c
 *
 *  Created on: Nov 2, 2022
 *      Author: khanh
 */
#include "Timer.h"
#include "../include.h"

//***************************************************************************************************************************
//                                                       VARIABLES DEFINE
//                                                   global variables in Timer.c
//***************************************************************************************************************************
static void Timer0_ISR(void);
static void Timer1_ISR(void);


//***************************************************************************************************************************
//                                                       TIMER CONFIGURATION
//***************************************************************************************************************************
extern void Config_Timer(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
//    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    /* Full-width timer must use TIMER_A */
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/1000 - 1);           //count from 0 -> -1, Interrupt after 1ms

    /* Timer for ADC 1ms Sampling */
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
//    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
//    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/1000 - 1);             //1ms interrupt

    /* Timer Interrupt config */
    TimerIntRegister(TIMER0_BASE, TIMER_A, &Timer0_ISR);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

//    TimerIntRegister(TIMER1_BASE, TIMER_A, &Timer1_ISR);
//    IntEnable(INT_TIMER1A);
//    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    /* Timer will be enable as soon as all ssr are closed*/
//    TimerEnable(TIMER0_BASE, TIMER_A);                                      //Enable Timer
}

//***************************************************************************************************************************
//                                                         TIMER FUNCTION
//***************************************************************************************************************************
static void Timer0_ISR()
{
    ADCProcessorTrigger(ADC0_BASE, 1);
    TimerDisable(TIMER0_BASE, TIMER_A);
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

static void Timer1_ISR()
{
    TimerDisable(TIMER1_BASE, TIMER_A);
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}
