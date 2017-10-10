
#include "main.h"

volatile uint8_t Temp_Save_Flag=0;
/***********************************************************************
函数名称：Manage_Mode
功    能：返回串口命令数据
输入参数：无
输出参数：无
编写时间：2017.5.27
编 写 人：
注    意：
***********************************************************************/
uint8_t Search_Mode(uint8_t *Buff,uint8_t Size)
{
	static uint8_t Mode=0;

	if(packerflag==1)
	{		
		packerflag=0;
		/* ???? */
		if(memcmp((void*)Buff,RXD_1,Size)==0)
		{
				Mode = StandBy_Mode;

		}
		/* ???? */
		if(memcmp((void*)Buff,Start_RXD_1,Size)==0)
		{
			if(Temp_Save_Flag!=1)
			{	
				Mode = Run_Mode;
				LED1_ON;
				#ifdef Use_Relay
				Relay1_On;
				Stop(100);
				Relay1_Off;
				#endif
				CAN_COMMAND.Laser_On_Off = Laser_ON;
			}
		}
		/* ???? */
		if(memcmp((void*)Buff,Stop_RXD_1,Size)==0)
		{
			Mode = Stop_Mode;
			LED1_OFF;
			#ifdef Use_Relay
			Relay2_On;
			Stop(100);
			Relay2_Off;
			#endif
			CAN_COMMAND.Laser_On_Off = Laser_OFF;
		}				
	}
		return Mode;
}


uint8_t Manage_Mode(uint8_t Mode)
{
	
	switch(Mode)
	{
		
		case StandBy_Mode:
			 if(memcmp((void*)RS232_buff,RXD_1,Uart_Size)==0)
				{
					RS232_Send_Data((void*)TXD_1,sizeof(TXD_1));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));
				}
				else if(memcmp((void*)RS232_buff,RXD_2,Uart_Size)==0)
				{
					RS232_Send_Data((void*)TXD_2,sizeof(TXD_2));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));
				}
				else if(memcmp((void*)RS232_buff,RXD_3,Uart_Size)==0)
				{
					RS232_Send_Data((void*)TXD_3,sizeof(TXD_3));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));		
				}
				else if(memcmp((void*)RS232_buff,RXD_4,Uart_Size)==0)
				{
					RS232_Send_Data((void*)TXD_4,sizeof(TXD_4));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));		
				}


				else if(memcmp((void*)RS232_buff,Start_RXD_2,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_2,sizeof(Start_TXD_2));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_3,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_3,sizeof(Start_TXD_3));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_4,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_4,sizeof(Start_TXD_4));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}		
				else if(memcmp((void*)RS232_buff,Start_RXD_5,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_5,sizeof(Start_TXD_5));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_6,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_6,sizeof(Start_TXD_6));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				
				else if(memcmp((void*)RS232_buff,Start_RXD_9,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_9,sizeof(Start_TXD_9));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_10,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_10,sizeof(Start_TXD_10));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_11,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_11,sizeof(Start_TXD_11));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_12,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_12,sizeof(Start_TXD_12));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_13,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_13,sizeof(Start_TXD_13));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_14,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_14,sizeof(Start_TXD_14));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_15,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_15,sizeof(Start_TXD_15));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_16,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_16,sizeof(Start_TXD_16));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_19,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_19,sizeof(Start_TXD_19));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_20,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_20,sizeof(Start_TXD_20));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_23,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_23,sizeof(Start_TXD_23));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}	
				
				else {}
				break;
				
			case Run_Mode:

				if(memcmp((void*)RS232_buff,Start_RXD_1,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_1,sizeof(Start_TXD_1));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));		
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_2,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_2,sizeof(Start_TXD_2));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_3,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_3,sizeof(Start_TXD_3));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_4,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_4,sizeof(Start_TXD_4));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}		
				else if(memcmp((void*)RS232_buff,Start_RXD_5,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_5,sizeof(Start_TXD_5));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_6,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_6,sizeof(Start_TXD_6));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_7,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_7,sizeof(Start_TXD_7));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_8,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_8,sizeof(Start_TXD_8));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_9,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_9,sizeof(Start_TXD_9));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_10,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_10,sizeof(Start_TXD_10));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_11,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_11,sizeof(Start_TXD_11));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_12,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_12,sizeof(Start_TXD_12));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_13,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_13,sizeof(Start_TXD_13));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_14,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_14,sizeof(Start_TXD_14));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_15,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_15,sizeof(Start_TXD_15));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_16,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_16,sizeof(Start_TXD_16));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_19,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_19,sizeof(Start_TXD_19));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_20,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_20,sizeof(Start_TXD_20));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_21,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_21,sizeof(Start_TXD_21));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_22,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_22,sizeof(Start_TXD_22));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_23,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_23,sizeof(Start_TXD_23));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}						
				else {}
				break;
				
				
			case Stop_Mode:
	//			LED1_OFF;
				if(memcmp((void*)RS232_buff,Stop_RXD_1,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Stop_TXD_1,sizeof(Stop_TXD_1));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));
				}	
				else if(memcmp((void*)RS232_buff,Stop_RXD_2,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Stop_TXD_2,sizeof(Stop_TXD_2));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));
				}
				else if(memcmp((void*)RS232_buff,Stop_RXD_3,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Stop_TXD_3,sizeof(Stop_TXD_3));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));		
				}
				else if(memcmp((void*)RS232_buff,Stop_RXD_4,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Stop_TXD_4,sizeof(Stop_TXD_4));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));
				}
				else if(memcmp((void*)RS232_buff,Stop_RXD_5,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Stop_TXD_5,sizeof(Stop_TXD_5));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));	
				}

				else if(memcmp((void*)RS232_buff,Start_RXD_2,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_2,sizeof(Start_TXD_2));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_3,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_3,sizeof(Start_TXD_3));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_4,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_4,sizeof(Start_TXD_4));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}		
				else if(memcmp((void*)RS232_buff,Start_RXD_5,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_5,sizeof(Start_TXD_5));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_6,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_6,sizeof(Start_TXD_6));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				
				else if(memcmp((void*)RS232_buff,Start_RXD_9,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_9,sizeof(Start_TXD_9));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_10,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_10,sizeof(Start_TXD_10));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_11,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_11,sizeof(Start_TXD_11));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_12,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_12,sizeof(Start_TXD_12));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_13,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_13,sizeof(Start_TXD_13));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_14,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_14,sizeof(Start_TXD_14));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_15,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_15,sizeof(Start_TXD_15));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_16,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_16,sizeof(Start_TXD_16));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_19,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_19,sizeof(Start_TXD_19));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_20,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_20,sizeof(Start_TXD_20));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}
				else if(memcmp((void*)RS232_buff,Start_RXD_23,Uart_Size)==0)
				{
					RS232_Send_Data((void*)Start_TXD_23,sizeof(Start_TXD_23));
					memset((void*)RS232_buff,0,sizeof(RS232_buff));			
				}	

				else {}
					
			break;
		default:
			break;
	}
	return 1;
}

/***********************************************************************
函数名称：Manage_Temp
功    能：历经60-70度情况下，温度再次大于70度，将关闭光源
输入参数：无
输出参数：返回1
编写时间：2017.5.27
编 写 人：
注    意：
***********************************************************************/
//uint8_t Manage_Temp(void)
//{
//	
//	static float Optical_Temp;
//	static float Ambient_Temp;
//	static uint8_t Temp_Statue;
//	Optical_Temp = DS18B20_ReadTempByID(g_rom_id);
////	Ambient_Temp = DS18B20_ReadTempByID_1(g_rom_id_1);
////		printf("The Temp1 is:%.4f\r\n",Optical_Temp);
////		printf("The Temp2 is:%.4f\r\n",Ambient_Temp);
//	if(Lower_Limit<(uint8_t)Optical_Temp<Upper_Limit)
//	{
//			Temp_Statue=1;
//	}

//	if((Optical_Temp>=Upper_Limit)&&(Temp_Statue==1))
//	{
//	
//			Temp_Statue=0;
//			Temp_Save_Flag=1;
//			LED1_OFF;
//			CAN_COMMAND.Laser_On_Off = Laser_OFF;
//			return 1;
//	}
//	if((Temp_Save_Flag)&&(Optical_Temp<70))
//	{
//		Temp_Save_Flag=0;
//	}
//	return 0;	
//}

uint8_t Manage_Temp(void)
{
	
	static uint8_t Optical_Temp;
	static uint8_t Ambient_Temp;
	static uint8_t Temp_Statue;
	if(sample_finish){
		sample_finish = 0;
		Optical_Temp = T_dis(sample_1);
		Ambient_Temp = T_dis(sample_2);
//		printf("Optical_Temp=%d\r\n",Optical_Temp);
//		printf("Ambient_Temp=%d\r\n",Ambient_Temp);
		#ifdef Use_Ambient
		CAN_COMMAND.Ambient_Temp = Ambient_Temp;
		#else
		CAN_COMMAND.Ambient_Temp = 25;
		#endif
	}
	
	if((Lower_Limit<(uint8_t)Optical_Temp)&&((uint8_t)Optical_Temp<Upper_Limit))
	{
			Temp_Statue=1;
	}

	if((Optical_Temp>=Upper_Limit)&&(Temp_Statue==1))
	{
	
			Temp_Statue=0;
			Temp_Save_Flag=1;
			LED1_OFF;
			CAN_COMMAND.Laser_On_Off = Laser_OFF;
			return 1;
	}
	if((Temp_Save_Flag)&&(Optical_Temp<70))
	{
		Temp_Save_Flag=0;
	}
	return 0;	
}
/***********************************************************************
函数名称：Manage_Temp
功    能：历经60-70度情况下，温度再次大于70度，将关闭光源
输入参数：无
输出参数：返回1
编写时间：2017.5.27
编 写 人：
注    意：
***********************************************************************/
void Manage_Light(void)
{
	static uint32_t lux;
	lux=calculateLux(0,NOM_INTEG_CYCLE);
//	printf("lux = %d\r\n",lux);
	if(lux<Light_Min)
	{
		LED1_OFF;
		CAN_COMMAND.Laser_On_Off = Laser_OFF;
	}
}

