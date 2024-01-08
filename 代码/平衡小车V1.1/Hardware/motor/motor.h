# ifndef _MOTOR_H
# define _MOTOR_h

# include "main.h"

# define PWM_MAX                            7200
# define PWM_MIN                            -7200 

# define AIN1_PIN                           GPIO_Pin_4
# define AIN1_PORT                          GPIOA
# define AIN1_CLK                           RCC_APB2Periph_GPIOA
# define AIN1_APBxPeriphClockCmd            RCC_APB2PeriphClockCmd

# define AIN2_PIN                           GPIO_Pin_5
# define AIN2_PORT                          GPIOA
# define AIN2_CLK                           RCC_APB2Periph_GPIOA
# define AIN2_APBxPeriphClockCmd            RCC_APB2PeriphClockCmd

# define BIN1_PIN                           GPIO_Pin_13
# define BIN1_PORT                          GPIOB
# define BIN1_CLK                           RCC_APB2Periph_GPIOB
# define BIN1_APBxPeriphClockCmd            RCC_APB2PeriphClockCmd

# define BIN2_PIN                           GPIO_Pin_12
# define BIN2_PORT                          GPIOB
# define BIN2_CLK                           RCC_APB2Periph_GPIOB
# define BIN2_APBxPeriphClockCmd            RCC_APB2PeriphClockCmd


# define Ain1(x)                            GPIO_WriteBit(AIN1_PORT, AIN1_PIN, (BitAction)x)
# define Ain2(x)                            GPIO_WriteBit(AIN2_PORT, AIN2_PIN, (BitAction)x) 
# define Bin1(x)                            GPIO_WriteBit(BIN1_PORT, BIN1_PIN, (BitAction)x) 
# define Bin2(x)                            GPIO_WriteBit(BIN2_PORT, BIN2_PIN, (BitAction)x) 

void Motor_Init(void);

void Limit(int *motorA, int *motorB);
int32_t abs(int32_t p);
void Load(int32_t motorA, int32_t motorB);

# endif
