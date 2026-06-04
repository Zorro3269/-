#include "openmv.h"

/**
  * @brief  OpenMV 信号接收引脚初始化
  */
void OpenMV_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 使能 GPIOB 时钟 (注意这里已经根据头文件的宏自动变成 GPIOB 了)
    RCC_APB2PeriphClockCmd(OPENMV_GPIO_CLK, ENABLE);

    // 2. 配置引脚：下拉输入 (IPD)
    // 下拉输入保证了 OpenMV 断线或者没发信号时，电平稳定为低，不会误报
    GPIO_InitStructure.GPIO_Pin = OPENMV_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(OPENMV_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  获取 OpenMV 的输出信号
  * @retval 1: 检测到目标信号 (高电平), 0: 未检测到信号 (低电平)
  */
uint8_t OpenMV_Get_Signal(void)
{
    // 读取 GPIO 输入状态
    if (GPIO_ReadInputDataBit(OPENMV_GPIO_PORT, OPENMV_GPIO_PIN) == Bit_SET)
    {
        return 1; // 接收到高电平信号
    }
    else
    {
        return 0; // 没有信号
    }
}
