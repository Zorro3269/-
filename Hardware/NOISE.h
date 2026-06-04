#ifndef __NOISE_H
#define __NOISE_H

#include "stm32f10x.h" 

// 声音传感器硬件宏定义 
#define NOISE_GPIO_CLK     (RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1)
#define NOISE_GPIO_PORT    GPIOA                 
#define NOISE_GPIO_PIN     GPIO_Pin_4            
#define NOISE_ADC_CHANNEL  ADC_Channel_4          
#define NOISE_ADC          ADC1                   

void NOISE_Init(void);

// 获取声音强度 (0-100%)
uint8_t Get_Sound_Intensity(void);

#endif /* __NOISE_H */
