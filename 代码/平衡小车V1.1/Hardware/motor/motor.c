/*
 * @Author: xgc030920@outlook.com
 * @Date: 2024-01-15 20:19:46
 * @LastEditors: xgc030920@outlook.com
 * @LastEditTime: 2024-01-15 20:23:41
 * @FilePath: \Project_EIDEc:\SyncFile\03-Project\04-平衡小车C8T6\代码\平衡小车V1.1\Hardware\motor\motor.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
# include "./motor/motor.h"
# include "./motor/motor_pwm.h"
# include "./motor/motor_encoder.h"

void Motor_GPIO_Config(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = AIN1_PIN | AIN2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BIN1_PIN | BIN2_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Motor_Init(void){

    Encoder_Init();
    Motor_GPIO_Config();
    PWM_Init();
}

void Limit(int *motorA, int *motorB){

    if (*motorA>PWM_MAX) *motorA=PWM_MAX;
    if (*motorA<PWM_MIN) *motorA=PWM_MIN;
    
    if (*motorB>PWM_MAX) *motorB=PWM_MAX;
    if (*motorB<PWM_MIN) *motorB=PWM_MIN;
}

/*
*函数名称：绝对值函数
*函数功能：通过与0比较，大于0则返回不变的值，小于0则返回相反的值
*入口参数：常规变量
*/
uint16_t abs(int p){

    uint16_t q;
    q = p>0?p:(-p);
    
    return q;
}

/*  nbvcx 
*函数名称：赋值函数
*函数功能：入口参数即为PID运算完成后的最终PWM值
*函数功能：电机A脉冲个数，电机B脉冲个数
*/
void Load(int motorA, int motorB){

    // 判断正负号，对应正反转
    if (motorA > 0)                     // 左电机
        Ain1(0), Ain2(1);       // 正转
    else 
        Ain1(1), Ain2(0);       // 反转
    
    TIM_SetCompare1(TIM1, abs(motorA));

    if (motorB > 0)                     // 右电机
        Bin1(0), Bin2(1);       // 正转
    else 
        Bin1(1), Bin2(0);       // 反转

    TIM_SetCompare4(TIM1, abs(motorB));
}
