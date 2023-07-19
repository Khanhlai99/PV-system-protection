/*
 * include.h
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

//c library
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

//driver API library

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"


//hardware library
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

//declarative library
#include "System/System.h"
#include "GPIO/GPIO.h"
#include "ADC/ADC.h"

#include "Protection/Local_protection.h"
//#include "Protection/"

/*define ports and pins for easier reading*/
#define PORTA               GPIO_PORTA_BASE
#define PORTB               GPIO_PORTB_BASE
#define PORTC               GPIO_PORTC_BASE
#define PORTD               GPIO_PORTD_BASE
#define PORTE               GPIO_PORTE_BASE
#define PORTF               GPIO_PORTF_BASE     //LEDS
#define PORTG               GPIO_PORTG_BASE
#define PORTH               GPIO_PORTH_BASE     //SSR_control
#define PORTJ               GPIO_PORTJ_BASE     //USER_SWITCH
#define PORTK               GPIO_PORTK_BASE     //SSR_control
#define PORTL               GPIO_PORTL_BASE
#define PORTM               GPIO_PORTM_BASE
#define PORTN               GPIO_PORTN_BASE
#define PORTP               GPIO_PORTP_BASE
#define PORTQ               GPIO_PORTQ_BASE

#define PIN7                GPIO_PIN_7
#define PIN6                GPIO_PIN_6
#define PIN5                GPIO_PIN_5
#define PIN4                GPIO_PIN_4
#define PIN3                GPIO_PIN_3
#define PIN2                GPIO_PIN_2
#define PIN1                GPIO_PIN_1
#define PIN0                GPIO_PIN_0

/*on/off status*/
typedef enum{
    off = 0,
    on
} state;

//GLOBAL VARIABLES
extern int position;
extern bool fault_flag;         // 0 ~ no fault, 1 ~ fault happens

extern int index;


#endif /* INCLUDE_H_ */
