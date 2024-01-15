/*
 * @Author: xgc030920@outlook.com
 * @Date: 2024-01-15 20:19:46
 * @LastEditors: xgc030920@outlook.com
 * @LastEditTime: 2024-01-15 20:28:16
 * @FilePath: \Project_EIDEc:\SyncFile\03-Project\04-平衡小车C8T6\代码\平衡小车V1.1\Hardware\gyro\gyro.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
# ifndef _GYRO_H
# define _GYRO_H

# include "main.h"
  
#define  Gyro_USART_TX_GPIO_PIN              GPIO_Pin_2  
#define  Gyro_USART_RX_GPIO_PIN              GPIO_Pin_3

// 加速度
typedef struct{

    float a_x;
    float a_y;
    float a_z;
}Acceleration;

// 角速度
typedef struct{

    float Roll;
    float Pitch;
    float Yaw;

}EulerAngle;

// 角度
typedef struct{

    float gyro_x;
    float gyro_y;
    float gyro_z;
}Gyro;

void Gyro_Init(void);
uint8_t Gyro_GetRxFlag(void);
void Control_GetAngle(Gyro *gyro, EulerAngle *euler, Acceleration *acc);

# endif
