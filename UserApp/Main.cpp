/*
 * @file:Main.c
 * @brief:
 * @Date: Mar 5, 2023
 * @Author: iTuring
 */

#include "common_inc.h"
#include <cstdio>

/**********************Main Entery *****************************/

EEPROMClass eeprom;

void Main(void)
{
    int i = 39;
    int j = 20;

	printf("max=%d\r\n",eeprom.Max(i, j));
}
