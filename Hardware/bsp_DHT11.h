#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "stm32f10x.h"                  

/*************** 引脚配置 (强烈建议换到 PB12) ****************/
#define DHT11_GPIO_PORT  GPIOB
#define DHT11_GPIO_PIN   GPIO_Pin_12
#define DHT11_GPIO_CLK   RCC_APB2Periph_GPIOB
/***********************************************************/		

//输出状态定义
#define OUT 1
#define IN  0

//控制DHT11引脚输出高低电平
#define DHT11_Low  GPIO_ResetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)
#define DHT11_High GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)

u8 DHT11_Init(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Bit(void);
void DHT11_Mode(u8 mode);
u8 DHT11_Check(void);
void DHT11_Rst(void);   

#endif
