/*
 * ADC.h
 *
 *  Created on: Oct 9, 2022
 *      Author: khanh
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

/***********************************Constant Variable********************************/
#define ref_voltage 3.178        //manual measuring the reference voltage of current sensor
#define ASCOffset   2050

/***********************************ADC PORT & PIN********************************/
#define ADC0        ADC0_BASE
#define PORTE      GPIO_PORTE_BASE
#define PORTD      GPIO_PORTD_BASE
#define PORTB      GPIO_PORTB_BASE
#define PORTK      GPIO_PORTK_BASE

/*Note
 * Voltage:
 *      PV          PK1     AIN17
 *      Boost       PE0     AIN3
 *      Load        PE1     AIN2
 *      BiDC        PE2     AIN1
 *      Batt        PE3     AIN0
 * Current:
 *      PV_1        PD7     AIN4
        PV_2        PD2     AIN13
        Boost_1     PE5     AIN8
        Boost_2     PE4     AIN9
        Load_1      PB4     AIN10
        Load_2      PB5     AIN11
        BiDC_1      PK3     AIN19
        BiDC_2      PK2     AIN18
        Batt_1      PD1     AIN14
        Batt_2      PD0     AIN15
 * */

/**********************************ADC FUNCTION******************************/
extern void Config_ADC(void);


#endif /* ADC_ADC_H_ */
