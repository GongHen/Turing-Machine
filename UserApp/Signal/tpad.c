/*
 * @file:tpad.c
 * @brief:
 * @Date: Mar 6, 2023
 * @Author: iTuring
 */

#include "tpad.h"
#include "delay.h"
#include "usart.h"



vu16 tpad_default_val=0;				//空载的时候(没有手按下),计数器需要的时间
