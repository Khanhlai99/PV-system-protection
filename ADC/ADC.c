/*
 * ADC.c
 *
 *  Created on: Oct 9, 2022
 *      Author: khanh
 *      Description: ADC sampling
 *      Formula: (Dig. Value (from ADC) - 2048 ) * 3.15 (Ref voltage) /(0.1*4095)
 *      2048 - Offset value
 *      3.15 - Ref. voltage
 *      0.1  - Scale factor of ASC712 20A, =.066 with 30A version
 *      4095 = 2^12 - 1, 12 bits ADC
 */

#include "../include.h"
#include "ADC.h"


//************************************************************************************
//                                   FUNCTION
//*************************************************************************************
void Analog_convert(uint32_t []);
void Config_sequencer(void);
uint32_t ADC0_dec0[8]={};
uint32_t ADC0_dec1[4]={};
uint32_t ADC0_dec2[3]={};

static void ADC0_ISR(void);

//***************************************************************************************************************************************
//                                                       VARIABLES DEFINE
//                                                   global variables in ADC.c
//***************************************************************************************************************************************
volatile float Voltage_PV[5], Voltage_Boost[5], Voltage_Load[5], Voltage_BiDC[5], Voltage_Batt[5];
volatile float Current_PV_1[5], Current_PV_2[5];
volatile float Current_Boost_1[5], Current_Boost_2[5], Current_Load_1[5], Current_Load_2[5];
volatile float Current_BiDC_1[5], Current_BiDC_2[5], Current_Batt_1[5], Current_Batt_2[5];


//***************************************************************************************************************************************
//                                                     ADC CONFIGURATION
//***************************************************************************************************************************************
extern void Config_ADC(void)
{
    //MODULE, PORT AND PIN
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);                                                 //Using Module ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);                                                //Using Port E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                                                //Using Port D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                                                //Using Port B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);                                                //Using Port K

    GPIOPinTypeADC(PORTE, PIN5 | PIN4 | PIN3 | PIN2 | PIN1 | PIN0);                             //Using PE0, PE1, PE2, PE3, PE4, PE5
    GPIOPinTypeADC(PORTD, PIN7 | PIN2 | PIN1 | PIN0 );                                          //Using PD7, PD2, PD1, PD0
    GPIOPinTypeADC(PORTB, PIN5 | PIN4 );                                                        //Using PB5, PB4
    GPIOPinTypeADC(PORTK, PIN3 | PIN2 | PIN1 | PIN0 );                                          //Using PK3, PK2, PK1, PK0

    ADCHardwareOversampleConfigure(ADC0, 64);
    //Sequencer configuration

    ADCSequenceConfigure(ADC0, 0, ADC_TRIGGER_PROCESSOR, 0);       //Using sequencer 0, ADC module triggered by processor clock
    ADCSequenceConfigure(ADC0, 1, ADC_TRIGGER_PROCESSOR, 1);       //Using sequencer 1, ADC module triggered by processor clock
    ADCSequenceConfigure(ADC0, 2, ADC_TRIGGER_PROCESSOR, 2);       //Using sequencer 2, ADC module triggered by processor clock

    ADCSequenceStepConfigure(ADC0, 0, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0, 0, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0, 0, 2, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0, 0, 3, ADC_CTL_CH3);
    ADCSequenceStepConfigure(ADC0, 0, 4, ADC_CTL_CH4);
    ADCSequenceStepConfigure(ADC0, 0, 5, ADC_CTL_CH13);
    ADCSequenceStepConfigure(ADC0, 0, 6, ADC_CTL_CH8);
    ADCSequenceStepConfigure(ADC0, 0, 7, ADC_CTL_CH9 | ADC_CTL_END);
//    ADCSequenceStepConfigure(ADC0, 0, 7, ADC_CTL_CH9);

    ADCSequenceStepConfigure(ADC0, 1, 0, ADC_CTL_CH10);
    ADCSequenceStepConfigure(ADC0, 1, 1, ADC_CTL_CH11);
    ADCSequenceStepConfigure(ADC0, 1, 2, ADC_CTL_CH19);
    ADCSequenceStepConfigure(ADC0, 1, 3, ADC_CTL_CH18 | ADC_CTL_END);
//    ADCSequenceStepConfigure(ADC0, 1, 3, ADC_CTL_CH18);

    ADCSequenceStepConfigure(ADC0, 2, 0, ADC_CTL_CH14);
    ADCSequenceStepConfigure(ADC0, 2, 1, ADC_CTL_CH15);
    ADCSequenceStepConfigure(ADC0, 2, 2, ADC_CTL_CH17 | ADC_CTL_IE | ADC_CTL_END);

    //Enable Sequence
    ADCSequenceEnable(ADC0, 0);
    ADCSequenceEnable(ADC0, 1);
    ADCSequenceEnable(ADC0, 2);


    //ADC INTERRUPT
//    ADCIntRegister(ADC0_BASE, 0, &ADC0_ISR);
//    ADCIntRegister(ADC0_BASE, 1, &ADC0_ISR);                                                    //ADC Interrupt-handler Register
    ADCIntRegister(ADC0_BASE, 2, &ADC0_ISR);
    ADCIntEnable(ADC0_BASE, 2);
}

//***************************************************************************************************************************************
//                                                              ADC FUNCTION
//***************************************************************************************************************************************

static void ADC0_ISR(void)                                                                      //attempt value from ADC
{
    ADCIntClear(ADC0_BASE, 2);

    ADCSequenceDataGet(ADC0_BASE, 0, (uint32_t*)&ADC0_dec0);
    ADCSequenceDataGet(ADC0_BASE, 1, (uint32_t*)&ADC0_dec1);
    ADCSequenceDataGet(ADC0_BASE, 2, (uint32_t*)&ADC0_dec2);

    if (position == 0){

        memmove((void*)&Voltage_PV[0], (void*)&Voltage_PV[1], 4 * sizeof(float));

        //current + shift
        memmove((void*)&Current_PV_1[0], (void*)&Current_PV_1[1], 4 * sizeof(float));
        //current - shift
        memmove((void*)&Current_PV_2[0], (void*)&Current_PV_2[1], 4 * sizeof(float));

        Voltage_PV[4]   = (float)(ADC0_dec2[2])*3/4095 * 100 -34.8;
//        Current_PV_1[4] = (float)(ADC0_dec0[4]);
        Current_PV_1[4] = ((float) ADC0_dec0[4]-2108)*ref_voltage/(0.1*4095);
//        Current_PV_2[4] = ((float)(ADC0_dec0[5])*ref_voltage/4095 - 1);

        fault_classify(local_measurement_protection(Current_PV_1, Voltage_PV));

        position++;
    }
    else if (position == 1){

        memmove((void*)&Voltage_Boost[0], (void*)&Voltage_Boost[1], 4 * sizeof(float));

        //current + shift
        memmove((void*)&Current_Boost_1[0], (void*)&Current_Boost_1[1], 4 * sizeof(float));

        //current - shift
        memmove((void*)&Current_Boost_2[0], (void*)&Current_Boost_2[1], 4 * sizeof(float));

        Voltage_Boost[4]   = (float)(ADC0_dec0[3])*3/4000*100 - 5.9;
//        Current_Boost_1[4] = (float)(ADC0_dec0[6]);
        Current_Boost_1[4] = ((float)ADC0_dec0[6]-2058)*ref_voltage/(0.1*4095);
        Current_Boost_2[4] = ((float)(ADC0_dec0[7])*ref_voltage/4095-1.1);

        fault_classify(local_measurement_protection(Current_Boost_1, Voltage_Boost));

        position++;
    }
    else if(position == 2){

        memmove((void*)&Voltage_Load[0], (void*)&Voltage_Load[1], 4 * sizeof(float));

       //current + shift
        memmove((void*)&Current_Load_1[0], (void*)&Current_Load_1[1], 4 * sizeof(float));

       //current - shift
        memmove((void*)&Current_Load_2[0], (void*)&Current_Load_2[1], 4 * sizeof(float));

       Voltage_Load[4]   = (float)(ADC0_dec0[2])*3/4155*100 - 8.5;
//       Current_Load_1[4] = (float)(ADC0_dec1[0]);
       Current_Load_1[4] = ((float)ADC0_dec1[0]-2092) * ref_voltage / (0.1*4095);
//       Current_Load_2[4] = (float)(ADC0_dec1[1] * ref_voltage / 4095 - 1.2);

        fault_classify(local_measurement_protection(Current_Load_1, Voltage_Load));

       position++;
    }
    else if(position == 3){

        memmove((void*)&Voltage_BiDC[0], (void*)&Voltage_BiDC[1], 4 * sizeof(float));

        //current + shift
        memmove((void*)&Current_BiDC_1[0], (void*)&Current_BiDC_1[1], 4 * sizeof(float));

        //current - shift
        memmove((void*)&Current_BiDC_2[0], (void*)&Current_BiDC_2[1], 4 * sizeof(float));

        Voltage_BiDC[4]   = (float)(ADC0_dec0[1])*3/4095*100-3.4;
        Current_BiDC_1[4] = ((float)(ADC0_dec1[2])*ref_voltage/4095-1.45);
        Current_BiDC_2[4] = ((float)(ADC0_dec1[3])*ref_voltage/4095-1.45);

//        fault_classify(local_measurement_protection(Current_BiDC_1, Voltage_BiDC));

        position++;
    }
    else if (position ==4){
//        ADCSequenceDataGet(ADC0_BASE, 0, (uint32_t*)&ADC0_dec0);
//        ADCSequenceDataGet(ADC0_BASE, 2, (uint32_t*)&ADC0_dec2);

        //voltage shift
        memmove((void*)&Voltage_Batt[0], (void*)&Voltage_Batt[1], 4 * sizeof(float));

        //current (+) shift
        memmove((void*)&Current_Batt_1[0], (void*)&Current_Batt_1[1], 4 * sizeof(float));

        //current (-) shift
        memmove((void*)&Current_Batt_2[0], (void*)&Current_Batt_2[1], 4 * sizeof(float));

        Voltage_Batt[4]   = (float)(ADC0_dec0[0])*3/4095*100-5.5;
        Current_Batt_1[4] = ((float)(ADC0_dec2[0])*ref_voltage/4095-1.4);
        Current_Batt_2[4] = ((float)(ADC0_dec2[1])*ref_voltage/4095-1.4);

//        fault_classify(local_measurement_protection(Current_Batt_1, Voltage_Batt));
        position++;
        if (index < 5000)
            index++;
    }

    if (position > 4)
        position = 0;

    start_ADC();
}

