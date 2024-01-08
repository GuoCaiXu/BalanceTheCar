# ifndef _MOTOR_ENCODER_H
# define _MOTOR_ENCODER_H

# include "main.h"

/*----------------------------编码器1----------------------------*/
# define ENCODER1_A_PIN                 GPIO_Pin_0
# define ENCODER1_B_PIN                 GPIO_Pin_1

# define ENCODER1_PORT                  GPIOA
# define ENCODER1_GPIO_APBClkCmd        RCC_APB2PeriphClockCmd
# define ENCODER1_CLK                   RCC_APB2Periph_GPIOA

# define ENCODER1_TIM                   TIM2
# define ENCODER1_TIM_APBClkCmd         RCC_APB1PeriphClockCmd
# define ENCODER1_TIM_CLK               RCC_APB1Periph_TIM2

/*----------------------------编码器2----------------------------*/

# define ENCODER2_A_PIN                 GPIO_Pin_6
# define ENCODER2_B_PIN                 GPIO_Pin_7

# define ENCODER2_PORT                  GPIOB
# define ENCODER2_GPIO_APBClkCmd        RCC_APB2PeriphClockCmd
# define ENCODER2_CLK                   RCC_APB2Periph_GPIOB

# define ENCODER2_TIM                   TIM4
# define ENCODER2_TIM_APBClkCmd         RCC_APB1PeriphClockCmd
# define ENCODER2_TIM_CLK               RCC_APB1Periph_TIM4

void Encoder_Motor_Init(void);
int Read_Speed(int TIMx);
void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);

# endif
