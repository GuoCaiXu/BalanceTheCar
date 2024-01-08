# include "./motor/motor.h"
# include "./motor/motor_pwm.h"
# include "./motor/motor_encoder.h"

void Motor_GPIO_Config(void){

    AIN1_APBxPeriphClockCmd(AIN1_CLK, ENABLE);
    AIN2_APBxPeriphClockCmd(AIN2_CLK, ENABLE);
    BIN1_APBxPeriphClockCmd(BIN1_CLK, ENABLE);
    BIN2_APBxPeriphClockCmd(BIN2_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = AIN1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AIN1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = AIN2_PIN;
    GPIO_Init(AIN2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BIN1_PIN;
    GPIO_Init(BIN1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BIN2_PIN;
    GPIO_Init(BIN2_PORT, &GPIO_InitStructure);
}

void Motor_Init(void){

    Motor_GPIO_Config();
    PWM_Init();
    Encoder_Motor_Init();
}

void Limit(int *motorA, int *motorB){

    if (*motorA>PWM_MAX) *motorA=PWM_MAX;
    if (*motorB>PWM_MAX) *motorB=PWM_MAX;

    if (*motorA<PWM_MIN) *motorA=PWM_MIN;
    if (*motorB<PWM_MIN) *motorB=PWM_MIN;
}

/*
*函数名称：绝对值函数
*函数功能：通过与0比较，大于0则返回不变的值，小于0则返回相反的值
*入口参数：常规变量
*/
int32_t abs(int32_t p){

    uint16_t q;
    q = p>0?p:(-p);
    
    return q;
}

/*  nbvcx 
*函数名称：赋值函数
*函数功能：入口参数即为PID运算完成后的最终PWM值
*函数功能：电机A脉冲个数，电机B脉冲个数
*/
void Load(int32_t motorA, int32_t motorB){

    // 判断正负号，对应正反转
    if (motorA > 0) 
        Ain1(0), Ain2(1);       // 正转
    else 
        Ain1(1), Ain2(0);       // 反转
    
    PWMA_SetCompare1(abs(motorA));

    if (motorB > 0)
        Bin1(1), Bin2(0);       // 正转
    else 
        Bin1(0), Bin2(1);       // 反转

    PWMB_SetCompare2(abs(motorB));
}
