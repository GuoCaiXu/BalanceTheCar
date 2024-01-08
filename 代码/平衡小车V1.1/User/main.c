# include "main.h"

extern Acceleration acc;
extern EulerAngle  euler;
extern Gyro         gyro;
extern int Vertical_out,Velocity_out,Turn_out;

extern int test1, test2, test3;

int main(void){

	OLED_Init();
	Gyro_Init();
	Motor_Init();
	NVIC_Config();
	TIM3_Init();

	OLED_ShowString(1, 1, "Angle: ");
	OLED_ShowString(3, 1, "Out: ");

	while(1){
		
		OLED_Showfloat(1, 8, euler.Roll);
		OLED_Showfloat(2, 2, euler.Pitch);
		OLED_Showfloat(2, 8, euler.Yaw);

		// OLED_Showfloat(3, 8, Vertical_out);
		// OLED_Showfloat(4, 2, Velocity_out);
		// OLED_Showfloat(4, 8, Turn_out);

		OLED_Showfloat(4, 2, test1);
		OLED_Showfloat(4, 8, test2);
		OLED_Showfloat(3, 8, test3);

		// OLED_Showfloat(3, 8, gyro.gyro_x);
		// OLED_Showfloat(4, 2, gyro.gyro_y);
		// OLED_Showfloat(4, 8, gyro.gyro_z);
	}
	
}
