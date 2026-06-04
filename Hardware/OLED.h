#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);

// 原来的8x16函数
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

// 新增的6x8函数
void OLED_ShowChar_6x8(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString_6x8(uint8_t Line, uint8_t Column, char *String);

#endif
