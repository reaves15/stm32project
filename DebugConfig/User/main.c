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
//float kp=0.3,ki=0.2,kd=0;
//float Target_A,Actual_A,Out_A;  //负数前进
//float error0_A,error1_A,errorint_A;
//float Target_B,Actual_B,Out_B;
//float error0_B,error1_B,errorint_B;
float Yaw;

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
	while (1)
	{  
		 IMU_0();
		 Yaw=((float)(Serial_RxPacket[13]| (Serial_RxPacket[14]<<8))) *  0.0054931640625f;
     OLED_ShowNum(1,5,Yaw,3);
	}
}


/*编码器测速测试*/
void TIM4_IRQHandler (void){
	 static uint16_t count;
   if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	 {     
		   count++;
		   if(count>=40)  //计数分频
			 {
			    count = 0;
				 	bt_Getdata();
		      if(bt_rxflag==1){MotorA_SetPWM(10);MotorB_SetPWM(10);/*PID_SetSpeed()*/;}
		      if(bt_rxflag==2){MotorA_SetPWM(0);MotorB_SetPWM(0);}
		      if(bt_rxflag==3){MotorA_SetPWM(-10);MotorB_SetPWM(-10);}
		      if(bt_rxflag==4){MotorA_SetPWM(10);MotorB_SetPWM(-10);}
		      if(bt_rxflag==5){MotorA_SetPWM(-10);MotorB_SetPWM(10);}
			    if(bt_rxflag==6){
				         int out=angle(Yaw,0,0);
				         int out1=PID_A(20+out);
				         int out2=PID_B(20-out);
				         MotorA_SetPWM(out1);
				         MotorB_SetPWM(out2);
			       }	 
			 }
       TIM_ClearITPendingBit(TIM4,TIM_IT_Update);		 
	 }
} 

