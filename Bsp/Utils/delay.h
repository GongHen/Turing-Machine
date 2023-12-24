/*
 * @file:delay.h
 * @brief:
 * @Date: Mar 5, 2023
 * @Author: iTuring
 */

#ifndef UTILS_DELAY_H_
#define UTILS_DELAY_H_

#include "sys.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);


#endif /* UTILS_DELAY_H_ */
