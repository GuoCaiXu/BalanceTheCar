# include "./motor/motor_encoder.h"

// 编码器1初始化函数
void Encoder_TIM2_Init(void){

    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;

    /*1.时钟配置*/
    ENCODER1_TIM_APBClkCmd(ENCODER1_TIM_CLK, ENABLE);
    ENCODER1_GPIO_APBClkCmd(ENCODER1_CLK, ENABLE);

    /*2.GPIO 配置*/
    GPIO_InitStructure.GPIO_Pin = ENCODER1_A_PIN | ENCODER1_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           // 浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER1_PORT, &GPIO_InitStructure);

    /*3.定时器配置*/
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(ENCODER1_TIM, &TIM_TimeBaseInitStructure);

    /*4.编码器配置: 定时器2，模式3，上上升沿*/
    TIM_EncoderInterfaceConfig(ENCODER1_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    /*5.输入捕获配置*/
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;          // 滤波器设置为10
    TIM_ICInit(ENCODER1_TIM, &TIM_ICInitStructure);

    /*6.清除定时器溢出更新标志位(清除计数值)*/
    TIM_ClearFlag(ENCODER1_TIM, TIM_FLAG_Update);

    /*7.定时器2，溢出更新，使能*/
    TIM_ITConfig(ENCODER1_TIM, TIM_IT_Update, ENABLE);

    /*8.定时器数据清0(输入捕获从0开始计数)*/
    TIM_SetCounter(ENCODER1_TIM, 0);

    /*9.定时器2使能*/
    TIM_Cmd(ENCODER1_TIM, ENABLE);
}

// 编码器2初始化函数
void Encoder_TIM4_Init(void){

    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;

    /*1.时钟配置*/
    ENCODER2_TIM_APBClkCmd(ENCODER2_TIM_CLK, ENABLE);
    ENCODER2_GPIO_APBClkCmd(ENCODER2_CLK, ENABLE);

    /*2.GPIO 配置*/
    GPIO_InitStructure.GPIO_Pin = ENCODER2_A_PIN | ENCODER2_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           // 浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER2_PORT, &GPIO_InitStructure);

    /*3.定时器配置*/
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(ENCODER2_TIM, &TIM_TimeBaseInitStructure);

    /*4.编码器配置: 定时器2，模式3，上上升沿*/
    TIM_EncoderInterfaceConfig(ENCODER2_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    /*5.输入捕获配置*/
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;          // 滤波器设置为10
    TIM_ICInit(ENCODER2_TIM, &TIM_ICInitStructure);

    /*6.清除定时器溢出更新标志位(清除计数值)*/
    TIM_ClearFlag(ENCODER2_TIM, TIM_FLAG_Update);

    /*7.定时器2，溢出更新，使能*/
    TIM_ITConfig(ENCODER2_TIM, TIM_IT_Update, ENABLE);

    /*8.定时器数据清0(输入捕获从0开始计数)*/
    TIM_SetCounter(ENCODER2_TIM, 0);

    /*9.定时器2使能*/
    TIM_Cmd(ENCODER2_TIM, ENABLE);
}

void Encoder_NVIC_Config(void){

    NVIC_InitTypeDef    NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

void Encoder_Motor_Init(void){

    Encoder_TIM2_Init();
    Encoder_TIM4_Init();
    Encoder_NVIC_Config();
}

// 编码器速度读取函数
// 入口参数：定时器
// 编码器产生的是脉冲，计数器计脉冲数(位移)
int Read_Speed(int TIMx){

    int value_1;

    switch(TIMx){

        case 2:{
            // 单周期位移作为速度值
            value_1 = (short)TIM_GetCounter(ENCODER1_TIM);
            TIM_SetCounter(ENCODER1_TIM, 0);
        }
        break;
        case 4:{
            // 单周期位移作为速度值
            value_1 = (short)TIM_GetCounter(ENCODER2_TIM);
            TIM_SetCounter(ENCODER2_TIM, 0);
        }
        break;
        default: value_1 = 0;
    }

    return value_1;
}

void TIM2_IRQHandler(void){

    if (TIM_GetITStatus(ENCODER1_TIM, TIM_IT_Update) == SET){           // 中断标志位1

        TIM_ClearITPendingBit(ENCODER1_TIM, TIM_IT_Update);             // 清除中断标志位
    }
}

void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(ENCODER2_TIM, TIM_IT_Update) == SET){           // 中断标志位1

        TIM_ClearITPendingBit(ENCODER2_TIM, TIM_IT_Update);             // 清除中断标志位
    }
}
