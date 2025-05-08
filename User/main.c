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

/*����������*/
float kp=0.3,ki=0.2,kd=0;
float Target_A,Actual_A,Out_A;  //����ǰ��
float error0_A,error1_A,errorint_A;
float Target_B,Actual_B,Out_B;
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
	OLED_ShowString(2, 1, "flag:");
	while (1)
	{  
		 IMU_0();
		 float Yaw=((float)(Serial_RxPacket[13]| (Serial_RxPacket[14]<<8))) *  0.0054931640625f;
     OLED_ShowNum(1,5,Yaw,3);
		 bt_Getdata();
		 OLED_ShowNum(2,6,bt_rxflag,1);
	}
}



/*���������ٲ���*/
void TIM4_IRQHandler (void){
	 static uint16_t count;
//	 float dx,dy;
//	 float Yaw0,Yaw1,dYaw;
   if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	 {     
		   count++;
		   if(count>=40)  //������Ƶ
			 {
			    count = 0;
				 	bt_Getdata();
		      if(bt_rxflag==1){MotorA_SetPWM(10);MotorB_SetPWM(10);/*PID_SetSpeed()*/;}
		      if(bt_rxflag==2){MotorA_SetPWM(0);MotorB_SetPWM(0);}
		      if(bt_rxflag==3){MotorA_SetPWM(-10);MotorB_SetPWM(-10);}
		      if(bt_rxflag==4){MotorA_SetPWM(10);MotorB_SetPWM(-10);}
		      if(bt_rxflag==5){MotorA_SetPWM(-10);MotorB_SetPWM(10);}
//          PID_SetSpeed();
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
	  
    Actual_A = EncoderA_Get();  //��ȡʵ���ٶ� 
    error1_A=error0_A;  //��ȡ���������ϴ����
		error0_A=Target_A-Actual_A;
		errorint_A+=error0_A; //������
		Out_A = kp*error0_A + ki*errorint_A + kd*(error0_A-error1_A);  //����pid
		if(Out_A>100){Out_A=100;}  //����޷�
		if(Out_A<-100){Out_A=-100;} 
		MotorA_SetPWM(-Out_A);
		
		Actual_B = EncoderB_Get();  //��ȡʵ���ٶ� 
		error1_B=error0_B;  //��ȡ���������ϴ����
		error0_B=Target_B-Actual_B;
		errorint_B+=error0_B; //������
		Out_B = kp*error0_B + ki*errorint_B + kd*(error0_B-error1_B);  //����pid
		if(Out_B>100){Out_B=100;}  //����޷�
		if(Out_B<-100){Out_B=-100;} 
		MotorB_SetPWM(Out_B);
}
