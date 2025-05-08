#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

//USART3_TX:PB10   ����ģ��HC05
//USART3_RX:PB11 
//USART2_TX:PA2    ������ģ��
//USART2_RX:PA3


uint8_t Serial_RxData3;
uint8_t Serial_RxFlag3;
uint8_t Serial_RxFlag2;
uint8_t Serial_RxPacket[16];				//����������ݰ�����


void Serial_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3, ENABLE);
	
	//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_initstructure;
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_initstructure);
	
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_initstructure);
	
	USART_InitTypeDef usartinit;
	usartinit.USART_BaudRate = 115200;
	usartinit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartinit.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usartinit.USART_Parity = USART_Parity_No;
	usartinit.USART_StopBits = USART_StopBits_1;
	usartinit.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&usartinit);
	
	//����RXNE��־λ��NVIC�����
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	//�������ȼ�Ϊ��2+2����������Ӧ���ȼ�������ռ���ȼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef nvicinit;
	nvicinit.NVIC_IRQChannel = USART2_IRQn;
	nvicinit.NVIC_IRQChannelCmd = ENABLE;
	nvicinit.NVIC_IRQChannelPreemptionPriority =0;
	nvicinit.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&nvicinit);
	
	
	USART_Cmd(USART2,ENABLE);
}


void Serial_SendByte(USART_TypeDef* USARTx,uint8_t Byte)
{
	USART_SendData(USARTx,Byte);
	
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
}
//����һ����
void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array,uint16_t length)
{
	uint16_t i;
	for(i=0;i<length;i++)
	{
		Serial_SendByte(USARTx,Array[i]);
	}
}
//����һ�ַ���
void Serial_SendString(USART_TypeDef* USARTx,char *String)  //�ַ����Դ�һ��������־λ��\0�����Բ��ô��ݳ���
{
	uint8_t i;
	for(i=0;String[i] != '\0';i++)
	{
		Serial_SendByte(USARTx,String[i]);
	}
}
//�η����� (X��Y�η�)
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result =1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}

//����һ����
void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(USARTx,Number/Serial_Pow(10,Length-i-1)%10+'0'); //��λ���У�������������ǰ�����~
	}
}
//��fputc�����ض��򵽴��ڡ���Ϊprintf�ײ��ǲ��ϵص���fputc��
int fputc(int ch,FILE *f)
{
	Serial_SendByte(USART2,ch);
	return ch;
}

uint8_t Serial_GetRxFlag3(void)
{
	if(Serial_RxFlag3==1)
	{
		Serial_RxFlag3 = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t Serial_GetRxFlag2(void)
{
	if(Serial_RxFlag2==1)
	{
		Serial_RxFlag2 = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData3;
}

void IMU_0(void)
{
	Serial_SendByte(USART2,0x05);
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		Serial_RxData3 = USART_ReceiveData(USART3);
		Serial_RxFlag3 = 1;
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	
	{
		uint8_t RxData = USART_ReceiveData(USART2);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
			
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
		if (RxState == 0)
		{
			if (RxData == 0x49)			//�������ȷʵ�ǰ�ͷ
			{
//				OLED_ShowString(3,1,"tou OK");
				RxState = 1;			//����һ��״̬
				pRxPacket = 0;			//���ݰ���λ�ù���
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ�����*/
		else if (RxState == 1)
		{
//			OLED_ShowString(4,1,"receive start");
			Serial_RxPacket[pRxPacket] = RxData;	//�����ݴ������ݰ������ָ��λ��
			pRxPacket ++;				//���ݰ���λ������
			if (pRxPacket >= 16)			//����չ�16������
			{
				RxState = 2;			//����һ��״̬
			}
		}
		/*��ǰ״̬Ϊ2���������ݰ���β*/
		else if (RxState == 2)
		{
			if (RxData == 0x4D)			//�������ȷʵ�ǰ�β��
			{
//				OLED_ShowString(3,6,"wei OK");
				RxState = 0;			//״̬��0
				Serial_RxFlag2 = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
			}
		}
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		//�����־λ
	}
}

