# include "./motor/motor_encoder.h"

// 编码器1初始化函数
void Encoder_TIM2_Init(void){

    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;

    /*1.时钟配置*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    /*2.GPIO 配置*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           // 浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*3.定时器配置*/
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    /*4.输入捕获配置*/
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 10;          // 滤波器设置为10
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    /*5.编码器配置: 定时器2，模式3，上上升沿*/
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    /*6.清除定时器溢出更新标志位(清除计数值)*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /*7.定时器2，溢出更新，使能*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /*8.定时器数据清0(输入捕获从0开始计数)*/
    TIM_SetCounter(TIM2, 0);

    /*9.定时器2使能*/
    TIM_Cmd(TIM2, ENABLE);
}

// 编码器2初始化函数
void Encoder_TIM4_Init(void){

    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;

    /*1.时钟配置*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    /*2.GPIO 配置*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           // 浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*3.定时器配置*/
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    /*4.编码器配置: 定时器2，模式3，上上升沿*/
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    /*5.输入捕获配置*/
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 10;          // 滤波器设置为10
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    /*6.清除定时器溢出更新标志位(清除计数值)*/
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);

    /*7.定时器2，溢出更新，使能*/
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    /*8.定时器数据清0(输入捕获从0开始计数)*/
    TIM_SetCounter(TIM4, 0);

    /*9.定时器2使能*/
    TIM_Cmd(TIM4, ENABLE);
}

void Encoder_Init(void){

    Encoder_TIM2_Init();
    Encoder_TIM4_Init();
}

// 编码器速度读取函数
// 入口参数：定时器
// 编码器产生的是脉冲，计数器计脉冲数(位移)
int Read_Speed(int TIMx){

    int value_1;

    switch(TIMx){

        case 2:{
            // 单周期位移作为速度值
            value_1 = (short)TIM_GetCounter(TIM2);
            TIM_SetCounter(TIM2, 0);
        }
        break;
        case 4:{
            // 单周期位移作为速度值
            value_1 = (short)TIM_GetCounter(TIM4);
            TIM_SetCounter(TIM4, 0);
        }
        break;
        default: value_1 = 0;
    }

    return value_1;
}

void TIM2_IRQHandler(void){

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){           // 中断标志位1

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);             // 清除中断标志位
    }
}

void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET){           // 中断标志位1

        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);             // 清除中断标志位
    }
}
