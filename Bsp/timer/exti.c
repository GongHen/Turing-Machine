/*
 * @file:exti.c
 * @brief:
 * @Date: Mar 6, 2023
 * @Author: iTuring
 */
#include "exti.h"
#include "delay.h"
#include "key.h"
#include "ov7725.h"

u8 ov_sta;	//帧中断标记
//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(ov_sta==0) {
      OV7725_WRST=0;  //复位写指针
      OV7725_WRST=1;
      OV7725_WREN=1;  //允许写入FIFO
      ov_sta++;    //帧中断加1
    }else OV7725_WREN=0;//禁止写入FIFO
}
