#include "stm32f10x.h"                  
#include "OLED.h"                       
#include <stdio.h>                      
// 头文件名字保持原来的 bsp_DHT11.h 不变
#include "bsp_DHT11.h"                  
#include "sgp30.h"                      
#include "Delay.h"                      
#include "NOISE.h"                      
#include "Buzzer.h"
#include "openmv.h"  

// ================= 宏定义配置 =================
#define ALARM_TEMP_THRESHOLD  30.0f  // 温度报警阈值

// ================= 全局变量区 =================
char Temp_chr[30];                      
float Temp1 = 0;                        
float Temp2 = 0;                        
u16 CO2_Value = 0;                      
u16 TVOC_Value = 0;                     
uint8_t Sound_Intensity = 0;            

int main(void)                          
{
    uint16_t time_count = 0;
    uint8_t openmv_signal_detected = 0; 

    // 1. 系统时钟与复用功能配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
    // 2. 外设模块初始化
    OLED_Init();                        
    SGP30_Init();                       
    NOISE_Init();                       
    Buzzer_Init();  
    OpenMV_Init(); 
    
    // 【必须新增】：调用新驱动的引脚初始化
    DHT11_Init();  
		
    // 3. OLED 静态UI框架显示 (开机只刷一次即可)
    OLED_ShowString_6x8(1, 1, "T:--.-C  H:--.-%   ");
    OLED_ShowString_6x8(2, 1, "CO2:--- TVOC:---   ");
    OLED_ShowString_6x8(3, 1, "Sound Level: 0%    "); 
		
    while(1)                            
    {
        // ================= 极速读取输入信号 =================
        openmv_signal_detected = OpenMV_Get_Signal(); 
        
        Sound_Intensity = Get_Sound_Intensity();
        sprintf(Temp_chr, "Sound Level: %d%%    ", Sound_Intensity);
        OLED_ShowString_6x8(3, 1, Temp_chr);      

// ================= 传感器低速读取 (1Hz) =================
        if (time_count % 5 == 0)
        {
            u8 temp_val = 0; 
            u8 humi_val = 0;
            
            __disable_irq(); // 屏蔽中断，确保时序不被打断
            uint8_t dht_status = DHT11_Read_Data(&temp_val, &humi_val); 
            __enable_irq();  // 恢复中断
            
            if(dht_status == 0) 
            {
                Temp1 = (float)temp_val; 
                Temp2 = (float)humi_val;
                sprintf(Temp_chr, "T:%dC  H:%d%%      ", temp_val, humi_val);
            }
            else // 读取失败，打印出具体的错误码！
            {
                sprintf(Temp_chr, "ERR Code: %d      ", dht_status); 
            }
            
            OLED_ShowString_6x8(1, 1, Temp_chr);  

            // 读取空气质量
            Sgp30_Get_Value(&CO2_Value, &TVOC_Value);
            sprintf(Temp_chr, "CO2:%d TVOC:%d    ", CO2_Value, TVOC_Value);
            OLED_ShowString_6x8(2, 1, Temp_chr);  
        }        
        // ================= 任务 3: 报警逻辑 =================
        if (Temp1 >= ALARM_TEMP_THRESHOLD || openmv_signal_detected == 1)
        {
            // 报警：利用 time_count 的奇偶性，实现 200ms响 / 200ms停 的“滴滴”声
            if (time_count % 2 == 0) 
            {
                Buzzer_ON();  
            }
            else 
            {
                Buzzer_OFF(); 
            }
        }
        else
        {
            Buzzer_OFF(); 
        }
				
        // ================= 任务 4: 系统节拍器 =================
        Delay_ms(200); 
        time_count++; 
        
        // 防溢出归零 (每 20 秒循环一次)
        if(time_count >= 100) 
        {
            time_count = 0;
        }
    }
}
