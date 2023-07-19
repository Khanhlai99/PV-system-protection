/*
 * System.c
 *
 *  Created on: April 18, 2023
 *      Author: khanh
 */

#include "../include.h"
#include "System.h"

extern uint8_t system_status = 0;

int32_t get_column_count(double array[]) {
    // Get the number of rows
    int cols;
//    *rows = sizeof(array) / sizeof(array[0]);

    // Get the number of columns
    cols = sizeof(array) / sizeof(array[0]);
    return cols;
}

void shift_values_to_previous_column(int array[], int cols) {
// Iterate over the columns
    int j = 1;
    while (j < cols) {
// Shift the value at the current column to the previous column
        array[j - 1] = array[j];
        j++;
    }
}

extern void fault_classify(int code_fault)
{
    switch (code_fault){
        case 0:                                            //pv fault detected
            ssr_control(0, off);
            LED_control(1, on);
            fault_flag = 1;
            break;
        case 1:                                            //boost fault detected
            ssr_control(1, off);
            LED_control(2, on);
            fault_flag = 1;
            break;
        case 2:                                            //load fault detected
            ssr_control(2, off);
            LED_control(3, on);
            fault_flag = 1;
            break;
        case 3:                                            //bi-dc fault detected
            ssr_control(3, off);
            LED_control(4, on);
            fault_flag = 1;
            break;
        case 4:                                            //battery fault detected
            ssr_control(4, off);
            LED_control(4, on);
            fault_flag = 1;
            break;
        case 5:                                            //all ssr off
            ssr_control(5, off);
            fault_flag = 1;
        default:                                           //no fault happen
            break;
    }
}

extern void start_ADC(void)
{
    ADCProcessorTrigger(ADC0_BASE, 0);
    ADCProcessorTrigger(ADC0_BASE, 1);
    ADCProcessorTrigger(ADC0_BASE, 2);

}
