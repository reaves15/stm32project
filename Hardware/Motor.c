#include "stm32f10x.h"                  // Device header
#include "PWM.h" 
#include "delay.h"
#include "math.h"
#include "Serial.h"
#include "delay.h"
#include "encoder.h"

float kp=0.3,ki=0.2,kd=0;

void MotorA_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	PWMA_Init();
}


void MotorA_SetPWM(int8_t PWM)
{
	if (PWM >= 0)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_15);//方向控制引脚  正转
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);
		PWMA_SetCompare(PWM);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_15);  //反转
		GPIO_SetBits(GPIOC,GPIO_Pin_14);
		PWMA_SetCompare(-PWM);
	}
}


void MotorB_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PWMB_Init();
}


void MotorB_SetPWM(int8_t PWM)
{
	if (PWM >= 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);//方向控制引脚  正转
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		PWMB_SetCompare(PWM);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);  //反转
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		PWMB_SetCompare(-PWM);
	}
}

float PID_A(float targetA){
	  float error0=0,error1=0,errorint=0;
	  float Target,Actual,Out;
	  Target=-targetA;
    Actual = EncoderA_Get();  //获取实际速度 
    error1=error0;  //获取本次误差和上次误差
		error0=Target-Actual;
		errorint+=error0; //误差积分
		Out = kp*error0 + ki*errorint + kd*(error0-error1);  //计算pid
		if(Out>100){Out=100;}  //输出限幅
		if(Out<-100){Out=-100;} 
		return -Out;
}

float PID_B(float targetB){
	  float error0=0,error1=0,errorint=0;
	  float Target,Actual,Out;
	  Target=targetB;
    Actual = EncoderB_Get();  //获取实际速度 
    error1=error0;  //获取本次误差和上次误差
		error0=Target-Actual;
		errorint+=error0; //误差积分
		Out = kp*error0 + ki*errorint + kd*(error0-error1);  //计算pid
		if(Out>100){Out=100;}  //输出限幅
		if(Out<-100){Out=-100;} 
		return Out;
}

int angle(float Angle,float Gyroy,float Mechanical_Angle)
{
	float Kp = 5; //       
  float Kd = 0;//      
	float Bias; //角度误差值
	int balance_up; //直立环控制PWM
	if(Angle>180){Angle=-(360-Angle);}
	Bias=Angle-Mechanical_Angle; //角度误差值==测量的俯仰角-理想角度（机械平衡角度）
	balance_up= Kp*Bias+ Kd*Gyroy; //计算平衡控制的电机PWM  PD控制   Up_balance_KP是P系数,Up_balance_KD是D系数
	return balance_up;
} 
