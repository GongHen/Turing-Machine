/*
 * @file:common_inc.h
 * @brief:
 * @Date: Mar 5, 2023
 * @Author: iTuring
 */

#ifndef COMMON_INC_H_
#define COMMON_INC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "interface_fsmc.h"
#include "sys.h"
#include "ov7725.h"
#include "delay.h"
#include "usart.h"
#include "sccb.h"
#include "lcd.h"
#include "exti.h"
#include "usmart.h"

extern void Main(void);

#ifdef __cplusplus
}

#include "emulated_eeprom.hpp"

#endif

#endif /* COMMON_INC_H_ */
