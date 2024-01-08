# include "./control/control.h"

Acceleration  acc;
EulerAngle    euler;
Gyro          gyro;
uint8_t       Pick_Up_Flag = 1;

float Med_Angle=0;      // 机械中值，能使得小车真正平衡住的角度 
float Target_Speed=0;	    // 期望速度。---二次开发接口，用于控制小车前进后退及其速度。
float Turn_Speed=0;       //期望速度（偏航）

// 直立环Kp、Kd
float Vertical_Kp= 200;    // 212
float Vertical_Kd= 5.4;     // 5.4

// 速度环Kp、Ki（正反馈）
float Velocity_Kp=0.67;    // 0.67                                                                                         
float Velocity_Ki=0.67/200;
 
float Turn_Kp=0;
float Turn_Kd=13;

int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输 出变量

int test1, test2, test3;

int Vertical(float Med,float Angle,float gyro);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z, int RC);
void Pick_Up_Test(void);

void TIM3_IRQHandler(void){

  int PWM_out;
  int Encoder_Left, Encoder_Right;
  int MOTO1, MOTO2;
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
    // 1.采集编码器数据&陀螺仪角度信息
    // 电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反
    Encoder_Left  = -Read_Speed(4); 
    Encoder_Right = Read_Speed(2);

    test1 = Encoder_Left;
    test2 = Encoder_Right;
    if (Gyro_GetRxFlag() == 1){
    
      Control_GetAngle(&gyro, &euler, &acc);
      //Pick_Up_Test();

      // 2.将数据压入闭环控制中，计算出控制输出量
      Vertical_out=Vertical(Med_Angle,euler.Roll, gyro.gyro_x);			    // 直立环
      Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);                 // 速度环
      Turn_out=Turn(gyro.gyro_z, Turn_Speed);	

      PWM_out=Vertical_out - Vertical_Kp * Velocity_out;//最终输出
      test3 = PWM_out;
    
      // 3.把控制输出量加载到电机上，完成最终控制
      MOTO1 = PWM_out-Turn_out; // 左电机
      MOTO2 = PWM_out+Turn_out; // 右电机
      Limit(&MOTO1,&MOTO2);     // PWM限幅
      if (Pick_Up_Flag == 1) Load(MOTO1,MOTO2);        // 加载到电机上
      else Load(0,0);
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

/*****************  
直立环PD控制器：Kp*Ek+Kd*Ek_D

入口：Med:机械中值(期望角度)，Angle:真实角度，gyro_Y:真实角速度
出口：直立环输出
******************/
int Vertical(float Med,float Angle,float gyro) 
{
  int PWM_out;
  
  PWM_out = Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro-0);
  
  return PWM_out;
}

/*****************  
速度环PI控制器：Kp*Ek+Ki*Ek_S(Ek_S：偏差的积分)
******************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
  // 定义成静态变量，保存在静态存储器，使得变量不丢掉
  static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;
  float a=0.7;
  
  if (Pick_Up_Flag == 0){

    PWM_out=Encoder_Err=Encoder_S=EnC_Err_Lowout=EnC_Err_Lowout_last=0;

    return 0;
  }
  else {
    // 1.计算速度偏差
    //舍去误差--我的理解：能够让速度为"0"的角度，就是机械中值。
    Encoder_Err = ((encoder_left+encoder_right)-Target);
    // 2.对速度偏差进行低通滤波
    // low_out = (1-a)*Ek+a*low_out_last
    EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_last; // 使得波形更加平滑，滤除高频干扰，放置速度突变
    EnC_Err_Lowout_last = EnC_Err_Lowout;   // 防止速度过大影响直立环的正常工作
    // 3.对速度偏差积分出位移
    Encoder_S+=EnC_Err_Lowout;
    // 4.积分限幅
    Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
    
    // 5.速度环控制输出
    PWM_out = Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
    
    return PWM_out;
  }
}

/*****************  
转向环：系数*Z轴角速度
******************/
int Turn(int gyro_Z, int RC)
{
  int PWM_out;
  
  PWM_out = gyro_Z*Turn_Kd + Turn_Kp*RC;
  
  return PWM_out;
}

/*****************  
小车拿起自动检测
******************/
void Pick_Up_Test(void){

  static uint16_t T_2 = 0, T_3 = 0;

  if (Pick_Up_Flag == 1){
    
    if ((Vertical_out > 5000) || (Vertical_out < -5000)) T_2 ++;
    else T_2 = 0;

    if (T_2 > 20){

      Pick_Up_Flag = 0;
      T_2 = 0;
    }
  }
  if (Pick_Up_Flag == 0){

    if ((euler.Roll < (5+Med_Angle) && (euler.Roll > (-5+Med_Angle)) && (gyro.gyro_x < 5) && (gyro.gyro_x > -5))){

      T_3 ++;
    }

    if (T_3 > 20){
      T_3 = 0;
      Pick_Up_Flag = 1;
    }
  }
}

