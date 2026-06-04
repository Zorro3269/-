#include "Buzzer.h"

/**
  * @brief  初始化蜂鸣器引脚 (PB12)
  */
void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUZZER_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    Buzzer_OFF(); // 初始化关闭
}

void Buzzer_ON(void)
{
    // 低电平触发：拉低引脚开启蜂鸣器
    GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN); 
}

void Buzzer_OFF(void)
{
    // 低电平触发：拉高引脚关闭蜂鸣器
    GPIO_SetBits(BUZZER_PORT, BUZZER_PIN); 
}
