#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h "
#include "timer.h"
#include "pwm.h"
#include "motor.h"
#include "encoder.h"
#include "OLED.h"
#include "hc05.h"
#include "serial.h"
#include "key.h"
#include "inv_mpu.h"
#include "math.h"

/*变量声明区*/
float kp=0.3,ki=0.2,kd=0;
float Target_A=-20,Actual_A,Out_A;  //负数前进
float error0_A,error1_A,errorint_A;
float Target_B=20,Actual_B,Out_B;
float error0_B,error1_B,errorint_B;

void PID_SetSpeed(void);

int main(void)
{
	OLED_Init();
	MotorA_Init();
	MotorB_Init();
	Timer_Init();
	EncoderA_Init();
	EncoderB_Init();
	Serial_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "Yaw:");
	OLED_ShowString(2, 1, "OUTA:");
	OLED_ShowString(3, 1, "OUTB:");
	while (1)
	{  
		 IMU_0();
		 float Yaw=((float)(Serial_RxPacket[13]| (Serial_RxPacket[14]<<8))) *  0.0054931640625f;
     OLED_ShowNum(1,5,Yaw,3);
		 bt_Getdata();
		 OLED_ShowNum(2,6,Out_A,3);
		 OLED_ShowNum(3,6,Out_B,3);
	}
}



/*编码器测速测试*/
void TIM4_IRQHandler (void){
	 static uint16_t count;
//	 float dx,dy;
//	 float Yaw0,Yaw1,dYaw;
   if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	 {     
		   count++;
		   if(count>=40)  //计数分频
			 {
			    count = 0;
            PID_SetSpeed();
//				  SpeedA= EncoderA_Get();
//				  SpeedB= EncoderB_Get();
//				  Yaw0 = Yaw1;
//				  Yaw1 = Yaw;
//				  dYaw =  Yaw1-Yaw0;
//				  Speed=(SpeedA+SpeedB)/2.0;
//				  s=Speed*0.04;
//				  dx=s*cos(dYaw);
//				  dy=s*sin(dYaw);
//          x=x+dx;
//          y=y+dy;				 
			 }
       TIM_ClearITPendingBit(TIM4,TIM_IT_Update);		 
	 }
} 


void PID_SetSpeed(void){
	  
    Actual_A = EncoderA_Get();  //获取实际速度 
    error1_A=error0_A;  //获取本次误差和上次误差
		error0_A=Target_A-Actual_A;
		errorint_A+=error0_A; //误差积分
		Out_A = kp*error0_A + ki*errorint_A + kd*(error0_A-error1_A);  //计算pid
		if(Out_A>100){Out_A=100;}  //输出限幅
		if(Out_A<-100){Out_A=-100;} 
		MotorA_SetPWM(-Out_A);
		
		Actual_B = EncoderB_Get();  //获取实际速度 
		error1_B=error0_B;  //获取本次误差和上次误差
		error0_B=Target_B-Actual_B;
		errorint_B+=error0_B; //误差积分
		Out_B = kp*error0_B + ki*errorint_B + kd*(error0_B-error1_B);  //计算pid
		if(Out_B>100){Out_B=100;}  //输出限幅
		if(Out_B<-100){Out_B=-100;} 
		MotorB_SetPWM(Out_B);
}
