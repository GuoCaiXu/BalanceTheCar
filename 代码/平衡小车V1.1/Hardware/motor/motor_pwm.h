# ifndef _MOTOR_PWM_H
# define _MOTOR_PWM_H

# include "main.h"            

/*配置ARR 和PSC*/
# define PWM_ARR_Period             (7200-1)
# define PWM_PSC_Prescaler          (1-1)

/*输出通道1*/
# define Left_PWM_OUT               GPIO_Pin_8

/*输出通道2*/
# define Right_PWM_OUT              GPIO_Pin_11   

void PWM_Init(void);

# endif
