#include "bsp_DHT11.h"

// 绝对精准的独立延时，彻底抛弃外部 Delay.h，防止被编译器优化
static void DHT11_Delay_us(uint32_t us)
{
    volatile uint32_t delay = 12 * us; // STM32F103 72MHz 下大约 1us
    while(delay--);
}
static void DHT11_Delay_ms(uint32_t ms)
{
    while(ms--) {
        DHT11_Delay_us(1000);
    }
}
			
//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_Mode(OUT); 	  //SET OUTPUT
	DHT11_Low; 	          //拉低DQ
	DHT11_Delay_ms(20);   //主机拉低18~30ms
	DHT11_High; 		  //DQ=1 
	DHT11_Delay_us(30);   //主机拉高10~40us
}

//等待DHT11的回应
//返回1:未检测到存在 (一直高电平)
//返回2:卡在低电平
//返回0:正常
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_Mode(IN);//SET INPUT	 
    while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		DHT11_Delay_us(1);
	};	 
	if(retry>=100) return 1; // 错误1：传感器装死，毫无反应
	else retry=0;
    
    while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		DHT11_Delay_us(1);
	};
	if(retry>=100) return 2; // 错误2：拉低了但没拉高
	return 0; // 正常响应
}

//从DHT11读取一个位
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//等待变为低电平
	{
		retry++;
		DHT11_Delay_us(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//等待变高电平
	{
		retry++;
		DHT11_Delay_us(1);
	}
	DHT11_Delay_us(40);//等待40us
	if(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN))return 1;
	else return 0;		   
}

//从DHT11读取一个字节
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}

//从DHT11读取一次数据
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
    u8 check_res = DHT11_Check();
	if(check_res == 0) // 如果响应正常
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
            return 0; // 成功
		}
        return 3; // 错误3：校验和错误
	}
	return check_res; // 返回错误1或2
}

u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;	
 	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);	 
 	GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure); 
 	GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN);	 
			    
	DHT11_Rst();  
	return DHT11_Check();
} 

void DHT11_Mode(u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(mode)
	{
		GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin =  DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}
