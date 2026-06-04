#ifndef __MOTOR_H
#define	__MOTOR_H
#include "stm32f10x.h"
#include "delay.h"

/***************** Zorro 的双驱小车底盘 ******************
 * 文件			:	双路 TB6612FNG 电机驱动及双编码器 h 文件                   
 * 接口			:	适配 STM32F103RCT6 及双 JGA25-370 编码器电机														
*********************************************************/

// --- 电机 A (左轮) 驱动引脚 ---
#define MOTOR_PWMA_PORT       GPIOA
#define MOTOR_PWMA_PIN        GPIO_Pin_8    // TIM1_CH1
#define MOTOR_PWMA_CLK        RCC_APB2Periph_GPIOA

#define MOTOR_DIR_PORT        GPIOB
#define MOTOR_AIN1_PIN        GPIO_Pin_12   
#define MOTOR_AIN2_PIN        GPIO_Pin_13   
#define MOTOR_DIR_CLK         RCC_APB2Periph_GPIOB

// --- 电机 B (右轮) 驱动引脚 ---
#define MOTOR_PWMB_PORT       GPIOA
#define MOTOR_PWMB_PIN        GPIO_Pin_11   // TIM1_CH4
#define MOTOR_PWMB_CLK        RCC_APB2Periph_GPIOA

#define MOTOR_BIN1_PIN        GPIO_Pin_14   
#define MOTOR_BIN2_PIN        GPIO_Pin_15   

// --- 编码器读取引脚配置 ---
#define ENCODER_PORT          GPIOA
#define ENCODER_CLK           RCC_APB2Periph_GPIOA
// 编码器 A (TIM3)
#define ENCODER_A_A_PIN       GPIO_Pin_6    // TIM3_CH1
#define ENCODER_A_B_PIN       GPIO_Pin_7    // TIM3_CH2
// 编码器 B (TIM2)
#define ENCODER_B_A_PIN       GPIO_Pin_0    // TIM2_CH1
#define ENCODER_B_B_PIN       GPIO_Pin_1    // TIM2_CH2


void MOTOR_Init(void);
void Motor_SetSpeed(int8_t Speed_A, int8_t Speed_B); // 同时设置双电机速度

void Encoder_Init(void);
int Read_Encoder_A(void); // 读取 A 轮速度
int Read_Encoder_B(void); // 读取 B 轮速度

#endif
