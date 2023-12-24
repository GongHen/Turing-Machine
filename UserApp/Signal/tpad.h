/*
 * @file:tpad.h
 * @brief:
 * @Date: Mar 6, 2023
 * @Author: iTuring
 */

#ifndef SIGNAL_TPAD_H_
#define SIGNAL_TPAD_H_

#include "sys.h"

//空载的时候(没有手按下),计数器需要的时间
//这个值应该在每次开机的时候被初始化一次
extern vu16 tpad_default_val;

void TPAD_Reset(void);
u16 TPAD_Get_Val(void);
u16 TPAD_Get_MaxVal(u8 n);
u8  TPAD_Init(u8 systick);
u8  TPAD_Scan(u8 mode);

#endif /* SIGNAL_TPAD_H_ */
