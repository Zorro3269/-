#ifndef __SGP30_H
#define	__SGP30_H

#include "stm32f10x.h"
#include "Delay.h"

// 1. 补充数据类型的简写
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

// 2. 补充原 sys.h 中的位带操作宏 
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

// 增加 GPIOC 的寄存器地址定义 (用于新的引脚)
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  

// 定义 PC 端口的位带操作宏，让 PCout 和 PCin 生效
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  // PC端口输出
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  // PC端口输入
// =====================================================================

/*************** 移植修改：避开编码器，移至空闲的 PC4 和 PC5 ****************/
// SGP30 GPIO宏定义
#define		SGP30_IIC_CLK										RCC_APB2Periph_GPIOC
#define 	SGP30_IIC_PORT									GPIOC
#define 	SGP30_IIC_SCL_PIN								GPIO_Pin_4	
#define 	SGP30_IIC_SDA_PIN								GPIO_Pin_5	

//IO操作函数	 
#define SGP30_IIC_SCL    PCout(4) 		//SCL
#define SGP30_IIC_SDA    PCout(5) 		//SDA	 
#define SGP30_READ_SDA   PCin(5) 		//输入SDA 
/*********************END**********************/

#define SGP30_read  0xb1                                    //SGP30的读地址
#define SGP30_write 0xb0                                    //SGP30的写地址

//SGP30
void SGP30_IIC_Start(void);				                          
void SGP30_IIC_Stop(void);	  			                        
void SGP30_IIC_Send_Byte(u8 txd);			                      
u16 SGP30_IIC_Read_Byte(unsigned char ack);                 
u8 SGP30_IIC_Wait_Ack(void); 				                        
void SGP30_IIC_Ack(void);					                          
void SGP30_IIC_NAck(void);				                          
void SGP30_Init(void);				  
void SGP30_Write(u8 a, u8 b);
u32 SGP30_Read(void);

// 同时获取CO2和TVOC
void Sgp30_Get_Value(u16 *co2_value, u16 *tvoc_value);

#endif
