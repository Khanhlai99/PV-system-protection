/*
 * Algorithm.c
 *
 *  Created on: Nov 14, 2022
 *      Author: khanh
 */

#include <Protection/Local_protection.h>
#include "../include.h"

float dt   = 0.001;
float Rf   = 0.5;
float L    = 0.01;
float R    = 0.2;
float C    = 0.025;


extern int local_measurement_protection (float current[5], float voltage[5])
{
    if ((index > 200)&& (system_status == 1)){
        float delta_i, delta_i2, delta_i2min;
        float delta_imax, delta_imin;
        float delta_v, delta_vmax, delta_vmin;

        delta_v = voltage[4] - voltage[3];
        delta_i = current[4] - current[3];

        delta_imax = voltage[4] / L * dt;
        delta_imin = (voltage[4] / L - current[4] * Rf / L) * dt;

        delta_i2 = current[4] + current[3] + current[1] + current[0] - 4 * current[2];
        delta_i2min = (((voltage[4] * Rf) / (L * L) + current[4] * (Rf * Rf) / (L * L)) * dt * dt) * 1.5;


        if (position != 4)
        {
            delta_vmin = 0.4 * voltage[4] * R * delta_imin;
            delta_vmax = 0.4 * voltage[4] * R * delta_imax;

        }
        else if (position == 4)
        {
            delta_vmax = 0.8 * voltage[4] * R * delta_imax;
            delta_vmin = 0.8 * voltage[4] * R * delta_imin;
        }


        if (current[4] < -0.7 || current[3] < -0.7)
        {
            if (fabs(delta_i) > 3 * fabs(delta_imax))
            {
                fault_flag = 1;
                return position;
            }
            else if (fabs(delta_i) > 3 * fabs(delta_imin))
            {
                if (fabs(delta_i2) > 3 * fabs(delta_i2min)){
                    fault_flag = 1;
                    return position;
                }
            }
        }
        //reliable coefficient 1.3
        else {
            if (delta_i > fabs(delta_imax))
            {
                fault_flag = 1;
                return position;
            }
                if (delta_i > 1.3 * fabs(delta_imin))
            {
                    if (fabs(delta_i2) > 1.3 * fabs(delta_i2min))
                    {
                        fault_flag = 1;
                        return position;
                    }
            }
        }

        if (fabs(delta_v) > fabs(delta_vmax))
        {
            if (fabs(delta_v) > fabs(delta_vmin))
            {
                fault_flag = 1;
                return position;
            }
        }
    }
     return 1000;
}
