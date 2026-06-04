#include "NOISE.h"
#include "Delay.h"

/**
 * @brief  初始化声音传感器引脚和ADC通道 (PA0)
 */
void NOISE_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(NOISE_GPIO_CLK, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 12MHz

    GPIO_InitStructure.GPIO_Pin = NOISE_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
    GPIO_Init(NOISE_GPIO_PORT, &GPIO_InitStructure);

    ADC_DeInit(NOISE_ADC);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(NOISE_ADC, &ADC_InitStructure);

    ADC_Cmd(NOISE_ADC, ENABLE);
    ADC_ResetCalibration(NOISE_ADC);
    while(ADC_GetResetCalibrationStatus(NOISE_ADC));
    ADC_StartCalibration(NOISE_ADC);
    while(ADC_GetCalibrationStatus(NOISE_ADC));
}

/**
 * @brief  包络检测算法：连续高速采样获取峰峰值，并映射为 0-100 的强度
 */
uint8_t Get_Sound_Intensity(void)
{
    uint16_t sample_val = 0;
    uint16_t max_val = 0;
    uint16_t min_val = 4095; 
    uint16_t i;
    
    ADC_RegularChannelConfig(NOISE_ADC, NOISE_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    // 高速连续采样 800 次，捕捉交流波形的包络
    for(i = 0; i < 800; i++)
    {
        ADC_SoftwareStartConvCmd(NOISE_ADC, ENABLE);                 
        while(!ADC_GetFlagStatus(NOISE_ADC, ADC_FLAG_EOC));          
        sample_val = ADC_GetConversionValue(NOISE_ADC);              
        ADC_ClearFlag(NOISE_ADC, ADC_FLAG_EOC);
        
        if(sample_val > max_val) max_val = sample_val;
        if(sample_val < min_val) min_val = sample_val;
    }

    uint16_t diff = max_val - min_val;

    // 【抗底噪阈值：80】波动差值小于80当做绝对安静处理 (可根据环境继续调大)
    if (diff < 80) return 0; 

    // 【满偏阈值：3200】降低灵敏度，声音要很大才能达到100%
    uint32_t intensity = (diff * 100) / 3200;
    
    if (intensity > 100) intensity = 100;

    return (uint8_t)intensity;
}
