#ifndef __OPENMV_H
#define __OPENMV_H

#include "stm32f10x.h"

// ================= 引脚宏定义 =================
// 已将 OpenMV 信号引脚安全地转移至 PB5
#define OPENMV_GPIO_PORT    GPIOB
#define OPENMV_GPIO_PIN     GPIO_Pin_5
#define OPENMV_GPIO_CLK     RCC_APB2Periph_GPIOB

// ================= 函数声明 =================
void OpenMV_Init(void);
uint8_t OpenMV_Get_Signal(void);

#endif /* __OPENMV_H */
