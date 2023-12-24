/*
 * @file:timer.c
 * @brief:
 * @Date: Mar 6, 2023
 * @Author: iTuring
 */
#include "timer.h"
#include "usart.h"

u8 ov_frame; 	//统计帧数
//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&htim6)) {
//		printf("frame:%dfps\r\n",ov_frame);	//打印帧率
		ov_frame=0;
    }
}
