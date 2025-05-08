#include "stm32f10x.h"                  // Device header
#include "PWM.h" 
#include "delay.h"
#include "math.h"
#include "Serial.h"
#include "delay.h"

#define TURN_SPEED        50    // ת���ٶ�
#define TURN_GAIN         5.0f   // ת�����ϵ��
#define YAW_TOLERANCE     2.0f   // �Ƕ��ݲ�
#define TARGET_ANGLE      90.0f  // Ŀ��Ƕ�

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
		GPIO_SetBits(GPIOC,GPIO_Pin_15);//�����������  ��ת
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);
		PWMA_SetCompare(PWM);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_15);  //��ת
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
		GPIO_SetBits(GPIOA,GPIO_Pin_5);//�����������  ��ת
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		PWMB_SetCompare(PWM);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);  //��ת
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		PWMB_SetCompare(-PWM);
	}
}


void Turn90Degrees(int direction) {
    
        MotorA_SetPWM(15);
			  MotorB_SetPWM(-15);

}
