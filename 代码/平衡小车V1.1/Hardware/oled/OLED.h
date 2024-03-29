#ifndef __OLED_H
#define __OLED_H

# include "stm32f10x.h"

/*引脚配置*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, OLED_SCL_PIN, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, OLED_SDA_PIN, (BitAction)(x))

# define OLED_SCL_PIN       GPIO_Pin_13
# define OLED_SDA_PIN       GPIO_Pin_12

/*OLED初始化*/
void OLED_Init(void);
/*OLED清屏*/
void OLED_Clear(void);
/*OLED显示一个字符*/
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
/*OLED显示字符串*/
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
/*OLED显示数字（十进制，正数）*/
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
/*OLED显示小数*/
void OLED_Showfloat(uint8_t Line, uint8_t Column, float Number);
/* OLED显示数字（十进制，带符号数）*/
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
/*OLED显示数字（十六进制，正数）*/
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
/*OLED显示数字（二进制，正数）*/
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif
