#include "Motor.h"

// ==================== 双路电机驱动部分 ====================
void MOTOR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// 1. 开启时钟：GPIOA(PWM), GPIOB(方向), TIM1(高级定时器)
	RCC_APB2PeriphClockCmd(MOTOR_PWMA_CLK | MOTOR_DIR_CLK | RCC_APB2Periph_TIM1, ENABLE);
	
	// 2. 初始化方向控制引脚 (PB12, PB13, PB14, PB15) 
	GPIO_InitStructure.GPIO_Pin = MOTOR_AIN1_PIN | MOTOR_AIN2_PIN | MOTOR_BIN1_PIN | MOTOR_BIN2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(MOTOR_DIR_PORT, &GPIO_InitStructure);
	
	// 3. 初始化 PWM 引脚 (PA8, PA11)
	GPIO_InitStructure.GPIO_Pin = MOTOR_PWMA_PIN | MOTOR_PWMB_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(MOTOR_PWMA_PORT, &GPIO_InitStructure);

	// 4. 配置 TIM1
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;			
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;		
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;			            
	
	// 初始化通道 1 (Motor A)
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	// 初始化通道 4 (Motor B)
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	TIM_Cmd(TIM1, ENABLE);
}	

void Motor_SetSpeed(int8_t Speed_A, int8_t Speed_B)
{
	// ----- 控制电机 A -----
	if(Speed_A > 0) {
		GPIO_SetBits(MOTOR_DIR_PORT, MOTOR_AIN1_PIN);
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_AIN2_PIN);
		TIM_SetCompare1(TIM1, Speed_A); 
	} else if(Speed_A < 0) {
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_AIN1_PIN);
		GPIO_SetBits(MOTOR_DIR_PORT, MOTOR_AIN2_PIN);
		TIM_SetCompare1(TIM1, -Speed_A);
	} else {
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_AIN1_PIN);
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_AIN2_PIN);
		TIM_SetCompare1(TIM1, 0);
	}
	
	// ----- 控制电机 B -----
	if(Speed_B > 0) {
		GPIO_SetBits(MOTOR_DIR_PORT, MOTOR_BIN1_PIN);
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_BIN2_PIN);
		TIM_SetCompare4(TIM1, Speed_B); // TIM1_CH4 对应 Compare4
	} else if(Speed_B < 0) {
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_BIN1_PIN);
		GPIO_SetBits(MOTOR_DIR_PORT, MOTOR_BIN2_PIN);
		TIM_SetCompare4(TIM1, -Speed_B);
	} else {
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_BIN1_PIN);
		GPIO_ResetBits(MOTOR_DIR_PORT, MOTOR_BIN2_PIN);
		TIM_SetCompare4(TIM1, 0);
	}
}

// ==================== 双路编码器读取部分 ====================
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(ENCODER_CLK, ENABLE);
	// 开启 TIM3 (编码器A) 和 TIM2 (编码器B) 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE); 

	// 初始化四个编码器引脚 (PA0, PA1, PA6, PA7) 为浮空输入
	GPIO_InitStructure.GPIO_Pin = ENCODER_A_A_PIN | ENCODER_A_B_PIN | ENCODER_B_A_PIN | ENCODER_B_B_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;  
	
	// --- 配置 TIM3 (电机A) ---
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10; 
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_SetCounter(TIM3, 0); 
	TIM_Cmd(TIM3, ENABLE);   

	// --- 配置 TIM2 (电机B) ---
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICInit(TIM2, &TIM_ICInitStructure); // 复用上面的滤波配置
	TIM_SetCounter(TIM2, 0); 
	TIM_Cmd(TIM2, ENABLE);   
}

int Read_Encoder_A(void)
{
	int count = (short)TIM_GetCounter(TIM3); 
	TIM_SetCounter(TIM3, 0);             
	return count;
}

int Read_Encoder_B(void)
{
	int count = (short)TIM_GetCounter(TIM2); 
	TIM_SetCounter(TIM2, 0);             
	return count;
}

