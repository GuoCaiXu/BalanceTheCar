/*
 * @Author: xgc030920@outlook.com
 * @Date: 2024-01-02 16:15:34
 * @LastEditors: xgc030920@outlook.com
 * @LastEditTime: 2024-01-15 15:20:02
 * @FilePath: \Project_EIDEc:\SyncFile\03-Project\04-平衡小车C8T6\代码\平衡小车V1.1\Hardware\motor\motor_pwm.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
# ifndef _MOTOR_PWM_H
# define _MOTOR_PWM_H

# include "main.h"            

/*输出通道1*/
# define Left_PWM_OUT               GPIO_Pin_8

/*输出通道2*/
# define Right_PWM_OUT              GPIO_Pin_11   

void PWM_Init(void);

# endif
