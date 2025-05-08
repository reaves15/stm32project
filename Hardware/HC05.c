#include "stm32f10x.h"                  // Device header
#include "serial.h"

uint8_t bt_rxdata;
uint8_t bt_rxflag;


void bt_Getdata(void){

	  if(Serial_GetRxFlag3()==1)
		{
	    bt_rxdata=Serial_GetRxData();
      if(bt_rxdata == 'f'){bt_rxflag=1;}
		  if(bt_rxdata == 's'){bt_rxflag=2;}
			if(bt_rxdata == 'b'){bt_rxflag=3;}
			if(bt_rxdata == 'l'){bt_rxflag=4;}
			if(bt_rxdata == 'r'){bt_rxflag=5;}
			if(bt_rxdata == 'a'){bt_rxflag=6;}
	  }
}

/*À¶ÑÀÒ£¿ØÄ£°å*/
//	  bt_Getdata();
//		if(bt_rxflag==1){ Target_A=-10; Target_B=10;PID_SetSpeed();}
//		if(bt_rxflag==2){MotorA_SetPWM(0);MotorB_SetPWM(0);}
//		if(bt_rxflag==3){Target_A=10; Target_B=-10;PID_SetSpeed();}
//		if(bt_rxflag==4){Turn90Degrees(1);}
//		if(bt_rxflag==5){Turn90Degrees(-1);}
