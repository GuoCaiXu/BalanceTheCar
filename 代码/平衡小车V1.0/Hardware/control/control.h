# ifndef _CONTROL_H
# define _CONTROL_H

# include "main.h"

typedef struct{

    float Roll;
    float Pitch;
    float Yaw;

}EulerAngle;

typedef struct{

    float gyro_x;
    float gyro_y;
    float gyro_z;
}Gyro;

extern Gyro         gyro;
extern EulerAngle   euler;
extern uint8_t      AngleGyro_Arr[];

void Control_Init(void);
void Control_GetAngle(void);
int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z, int RC);
void Pick_Up_Test(void);
uint8_t Control_PID(void);

# endif
