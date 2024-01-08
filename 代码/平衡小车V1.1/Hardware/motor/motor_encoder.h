# ifndef _MOTOR_ENCODER_H
# define _MOTOR_ENCODER_H

# include "main.h"

void Encoder_Motor_Init(void);
int Read_Speed(int TIMx);
void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);

# endif
