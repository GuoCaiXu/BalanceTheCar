# ifndef _MOTOR_H
# define _MOTOR_h

# include "main.h"

# define PWM_MAX                            7200
# define PWM_MIN                            -7200 

# define AIN1_PIN                           GPIO_Pin_13
# define AIN1_PORT                          GPIOC

# define AIN2_PIN                           GPIO_Pin_14
# define AIN2_PORT                          GPIOC

# define BIN1_PIN                           GPIO_Pin_9
# define BIN1_PORT                          GPIOB

# define BIN2_PIN                           GPIO_Pin_8
# define BIN2_PORT                          GPIOB

# define Ain1(x)                            GPIO_WriteBit(AIN1_PORT, AIN1_PIN, (BitAction)x)
# define Ain2(x)                            GPIO_WriteBit(AIN2_PORT, AIN2_PIN, (BitAction)x) 
# define Bin1(x)                            GPIO_WriteBit(BIN1_PORT, BIN1_PIN, (BitAction)x) 
# define Bin2(x)                            GPIO_WriteBit(BIN2_PORT, BIN2_PIN, (BitAction)x) 

void Motor_Init(void);

void Limit(int *motorA, int *motorB);
void Load(int motorA, int motorB);

# endif
