#include "stm32f10x.h"                  // Device header

void LED_Init()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //GPIOC外设时钟配置
  
	GPIO_InitTypeDef GPIO_InitStructure;    //GPIO模式配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_2 | GPIO_Pin_9);
}

void LED1_On()
{
  GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
}
	
void LED1_Off()
{
  GPIO_SetBits(GPIOA,GPIO_Pin_1); 
}

void LED2_On()
{
  GPIO_ResetBits(GPIOA,GPIO_Pin_2); 
}

void LED2_Off()
{
  GPIO_SetBits(GPIOA,GPIO_Pin_2); 
}

void LED1_Turn(void)
{
   if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_9) == 0)
	 {
	   GPIO_SetBits(GPIOA,GPIO_Pin_9);
	 }
	 else
	 {
	   GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	 }
}

void LED2_Turn(void)
{
   if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2) == 0)
	 {
	   GPIO_SetBits(GPIOA,GPIO_Pin_2);
	 }
	 else
	 {
	   GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	 }
}
