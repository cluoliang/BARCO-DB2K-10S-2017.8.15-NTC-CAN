#include "main.h"
volatile CAN_Trans_Byte CAN_COMMAND={0x01,0x02,0x02,0x02,0x19,0x19};
CanTxMsg TxMessage;
uint8_t Can_Recieve_Flag;
void CAN_Config(void)
 {
	
  GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;


   /* CAN GPIOs configuration **************************************************/
 
   /* Enable GPIO clock */
   RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);
 
   /* Connect CAN pins to AF9 */
   GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
   GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
   
   /* Configure CAN RX and TX pins */
   GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);
 
   /* CAN configuration ********************************************************/  
   /* Enable CAN clock */
   RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
   
   /* CAN register init */
   CAN_DeInit(CANx);
 
   /* CAN cell init */
   CAN_InitStructure.CAN_TTCM = DISABLE;
   CAN_InitStructure.CAN_ABOM = DISABLE;
   CAN_InitStructure.CAN_AWUM = DISABLE;
   CAN_InitStructure.CAN_NART = DISABLE;
   CAN_InitStructure.CAN_RFLM = DISABLE;
   CAN_InitStructure.CAN_TXFP = DISABLE;
   CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
   CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
     
   /* CAN Baudrate = 500KBps (CAN clocked at 30 MHz) */
   CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
   CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
   CAN_InitStructure.CAN_Prescaler = 4;
   CAN_Init(CANx, &CAN_InitStructure);
 
   /* CAN filter init */
 #ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
 #else /* USE_CAN2 */
   CAN_FilterInitStructure.CAN_FilterNumber = 14;
 #endif  /* USE_CAN1 */
   CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
   CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
   CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
   CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
   CAN_FilterInit(&CAN_FilterInitStructure);
   
   /* Transmit Structure preparation */
   /* Enable FIFO 0 message pending Interrupt */
   CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: Manage_Can_Receive
*	功能说明: 处理Can接收中断,接收到主机的命令后,返回需要的数据
*	形    参: Can_Recieve_Flag-->接收命令标志位
*	返 回 值: uint8_t 顺利执行,返回1
*********************************************************************************************************
*/
uint8_t Manage_Can_Receive(uint8_t Can_Recieve_Flag)
{
	if((Can_Recieve_Flag!=1)&&(Can_Recieve_Flag!=0))
	return 0;
	if(Can_Recieve_Flag)
	{
		Can_Recieve_Flag=Clear_Flag;
//		Manage_Temp();
		Init_TxMes(&TxMessage);
	}
	return 1;
}
/*
*********************************************************************************************************
*	函 数 名: Init_TxMes
*	功能说明: Can总线发送数据
*	形    参: TxMessage-->发送结构体
*	返 回 值: 无
*********************************************************************************************************
*/
void Init_TxMes(CanTxMsg *TxMessage)
{
	TxMessage->StdId = 0x03;
	TxMessage->ExtId = 0;
	TxMessage->IDE = CAN_ID_STD;
	TxMessage->RTR = CAN_RTR_DATA;
	TxMessage->DLC = 8;
	
	TxMessage->Data[0] = 0xFE;
	TxMessage->Data[1] = CAN_COMMAND.Laser_On_Off;
	TxMessage->Data[2] = CAN_COMMAND.Display_Mode;
	TxMessage->Data[3] = CAN_COMMAND.Exter_Switch;
	TxMessage->Data[4] = CAN_COMMAND.Ambient_Light;
	TxMessage->Data[5] = CAN_COMMAND.Optical_Temp;
	TxMessage->Data[6] = CAN_COMMAND.Ambient_Temp;
	TxMessage->Data[7] = 0xFF;
	
	CAN_Transmit(CANx, TxMessage);
}
