/*
 * @Author: xgc030920@outlook.com
 * @Date: 2024-01-02 16:43:40
 * @LastEditors: xgc030920@outlook.com
 * @LastEditTime: 2024-01-15 20:29:07
 * @FilePath: \Project_EIDEc:\SyncFile\03-Project\04-平衡小车C8T6\代码\平衡小车V1.0\Hardware\gyro\gyro.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
# include "./gyro/gyro.h"

uint8_t     AngleGyro_Arr[18];
uint8_t     Serial_RxFlag;

void Gyro_USART_Config(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // 1. GPIO初始化
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = Gyro_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Gyro_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 2. 串口初始化
    USART_InitTypeDef   USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;                                        //配置波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //硬件流控制选择
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                     //串口模式
    USART_InitStructure.USART_Parity = USART_Parity_No;                                 //选择是否使用奇偶校验位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                              //停止位长度
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                         //配置字长
    USART_Init(USART2, &USART_InitStructure);

    // 3. 中断初始化
    NVIC_InitTypeDef    NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 陀螺仪串口
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    //串口接收中断需要用的
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);   
}

// 陀螺仪初始化
void Gyro_Init(void){

    Gyro_USART_Config();
}

// 获取角度 角速度 加速度
void Control_GetAngle(Gyro *gyro, EulerAngle *euler, Acceleration *acc){

    acc->a_x     = ((int16_t)((AngleGyro_Arr[1] << 8) | AngleGyro_Arr[0]) / 32768.0f) * 156;
    acc->a_y     = ((int16_t)((AngleGyro_Arr[3] << 8) | AngleGyro_Arr[2]) / 32768.0f) * 156;
    acc->a_z     = ((int16_t)((AngleGyro_Arr[5] << 8) | AngleGyro_Arr[4]) / 32768.0f) * 156;
    gyro->gyro_x = ((int16_t)((AngleGyro_Arr[7] << 8) | AngleGyro_Arr[6]) / 32768.0f) * 2000;
    gyro->gyro_y = ((int16_t)((AngleGyro_Arr[9] << 8) | AngleGyro_Arr[8]) / 32768.0f) * 2000;
    gyro->gyro_z = ((int16_t)((AngleGyro_Arr[11] << 8) | AngleGyro_Arr[10]) / 32768.0f) * 2000;
    euler->Roll  = ((int16_t)((AngleGyro_Arr[13] << 8) | AngleGyro_Arr[12]) / 32768.0f) * 180;
    euler->Pitch = ((int16_t)((AngleGyro_Arr[15] << 8) | AngleGyro_Arr[14]) / 32768.0f) * 180;
    euler->Yaw   = ((int16_t)((AngleGyro_Arr[17] << 8) | AngleGyro_Arr[16]) / 32768.0f) * 180;
    memset(AngleGyro_Arr, 18, 0x00);
}

// 获取串口接收中断标志位
uint8_t Gyro_GetRxFlag(void){

    if (Serial_RxFlag == 1){

        Serial_RxFlag = 0;
        return 1;
    }

    return 0;
}

// 外部中断函数
void USART2_IRQHandler(void){

    static uint8_t RxState = 0;
    static uint8_t RxClassState = 0;
    static uint8_t pRxPacket = 0;

    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET){

        uint8_t RxData = USART_ReceiveData(USART2);

        if (RxState == 0){
            if (RxData == 0x55){
                RxState = 1;
            }
            else RxState = 0;
        }
        else if (RxState == 1){
            if (RxData == 0x51){
                RxClassState = 0;
                RxState = 2;
            }
            else if (RxData == 0x52){
                RxClassState = 1;
                RxState = 2;
            }
            else if (RxData == 0x53){
                RxClassState = 2;
                RxState = 2;
            }
            else RxState = 0;
        }
        else if (RxState == 2){
            AngleGyro_Arr[6*RxClassState+pRxPacket] = RxData;
            pRxPacket ++;

            if (pRxPacket >= 6){
                if (RxClassState == 2) Serial_RxFlag = 1;
                pRxPacket = 0;
                RxState = 0;
            }
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
