/**
 ******************************************************************************
 * @file           : main.c
 * @author         : David Ramírez Betancourth
 * @brief          : Factorial number
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>


int main(void)
{
    

	uint32_t num = 5;
    volatile uint32_t res = 1;


    for(uint8_t i = 1; i <= num; i++) {
        if (num == 1) {
            return num;
        }
        else {
            res *= i;
        }

    }

    printf("----------------\nFactorial of %u is %u\n----------------------", num, res);
    return 0;
}
