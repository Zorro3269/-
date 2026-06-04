#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"

// ·äÃùÆ÷Òý½Åºê¶¨Òå
#define BUZZER_PORT     GPIOB
#define BUZZER_PIN      GPIO_Pin_12
#define BUZZER_CLK      RCC_APB2Periph_GPIOB

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);

#endif
