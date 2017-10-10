#include "main.h"

DMA_InitTypeDef sEEDMA_InitStructure;   
uint8_t  HEADER_ADDRESS_Write = (((SLAVE_ADDRESS & 0xFF00) >> 7) | 0xF0); 
uint8_t  HEADER_ADDRESS_Read = (((SLAVE_ADDRESS & 0xFF00) >> 7) | 0xF1);
uint8_t  NumberOfByteToReceive; 
uint8_t  NumberOfByteToTransmit; 
volatile uint8_t  TxBuffer []; 
volatile uint8_t  RxBuffer []; 
volatile uint8_t  Counter = 0x00;
volatile uint32_t  TimeOut; 
volatile uint32_t  Event = 0x00;  
volatile uint8_t  Tx_Idx; 
volatile uint8_t  Rx_Idx;
volatile uint8_t  GenerateStartStatus = 0x00; 
volatile uint8_t  MasterMode; 
volatile uint16_t sEEAddress = 0;
volatile uint16_t* sEEDataReadPointer;
volatile uint16_t* sEEDataReadPointer; 
volatile uint8_t *  sEEDataWritePointer ;
volatile uint32_t  sEETimeout; 
volatile uint8_t  sEEDataNum;

/*********************************************************************************************************
*	函 数 名: I2C_GPIO_Config
*	功能说明: 配置I2C接口 PB7---SDA PB6---SCLK
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*I2C GPIO clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);	
		
	/* GPIO Configuration */
	/*Configure I2C SCL pin */
	GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN|I2Cx_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
	
}

/*********************************************************************************************************
*	函 数 名: I2C_Config
*	功能说明: 配置I2C
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void I2C_Config(void)
{

	I2C_InitTypeDef  I2C_InitStructure;
	/*I2C Peripheral clock enable */
	RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
#if defined (I2C_MASTER)
	I2C_DeInit(I2Cx);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
	I2C_InitStructure.I2C_OwnAddress1 = 0x0A;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

#ifndef I2C_10BITS_ADDRESS
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
#else
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
#endif /* I2C_10BITS_ADDRESS */	
	/* I2C Initialization */
	I2C_Init(I2Cx, &I2C_InitStructure); 	
	I2C_Cmd(I2Cx, ENABLE); 	
	
#endif
}

/*********************************************************************************************************
*	函 数 名: sEE_DeInit
*	功能说明: 初始化之前复位I2C
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit(); 
}

/*********************************************************************************************************
*	函 数 名: sEE_Init
*	功能说明: 初始化I2C
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void sEE_Init(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	sEE_LowLevel_Init();
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
	I2C_Cmd(sEE_I2C, ENABLE);
  I2C_Init(sEE_I2C, &I2C_InitStructure);
  I2C_DMACmd(sEE_I2C, ENABLE);
  sEEAddress = sEE_HW_ADDRESS; 

}
/*********************************************************************************************************
*	函 数 名: sEE_LowLevel_DeInit
*	功能说明: 复位I2C
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void sEE_LowLevel_DeInit(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure; 
		
	/* sEE_I2C Peripheral Disable */
	I2C_Cmd(sEE_I2C, DISABLE);
	/* sEE_I2C DeInit */
	 I2C_DeInit(sEE_I2C);
	 
	/*!< sEE_I2C Periph clock disable */
	RCC_APB1PeriphClockCmd(sEE_I2C_CLK, DISABLE);
			
	/*!< GPIO configuration */  
	/*!< Configure sEE_I2C pins: SCL */
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/*!< Configure sEE_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
	GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

		/*先不要修改，统一在NVIC函数里面修改*/
	///* Configure and enable I2C DMA TX Stream interrupt */
	//NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	//NVIC_Init(&NVIC_InitStructure);

	///* Configure and enable I2C DMA RX Stream interrupt */
	//NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
	//NVIC_Init(&NVIC_InitStructure);   
		 
	/* Disable and Deinitialize the DMA Streams */
	DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
	DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
	DMA_DeInit(sEE_I2C_DMA_STREAM_TX);
	DMA_DeInit(sEE_I2C_DMA_STREAM_RX);
 }

/*********************************************************************************************************
*	函 数 名: sEE_LowLevel_Init
*	功能说明: 配置I2C和初步配置DMA
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void sEE_LowLevel_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
 
	/*!< sEE_I2C Periph clock enable */
	RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);
		 
	/*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
	RCC_AHB1PeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	 
	/* Reset sEE_I2C IP */
	RCC_APB1PeriphResetCmd(sEE_I2C_CLK, ENABLE);
	 
	/* Release reset signal of sEE_I2C IP */
	RCC_APB1PeriphResetCmd(sEE_I2C_CLK, DISABLE);
			
	/*!< GPIO configuration */
	/* Connect PXx to I2C_SCL*/
	GPIO_PinAFConfig(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_SOURCE, sEE_I2C_SCL_AF);
	/* Connect PXx to I2C_SDA*/
	GPIO_PinAFConfig(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_SOURCE, sEE_I2C_SDA_AF);  
		
	/*!< Configure sEE_I2C pins: SCL */   
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	 /*!< Configure sEE_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
	GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	 
	/*后面统一在NVIC中修改*/	 
//	/* Configure and enable I2C DMA TX Channel interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	/* Configure and enable I2C DMA RX Channel interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
//	NVIC_Init(&NVIC_InitStructure);  
		 
	/*!< I2C DMA TX and RX channels configuration */
	/* Enable the DMA clock */
	RCC_AHB1PeriphClockCmd(sEE_I2C_DMA_CLK, ENABLE);
		 
	/* Clear any pending flag on Rx Stream  */
	DMA_ClearFlag(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_FEIF | sEE_TX_DMA_FLAG_DMEIF | sEE_TX_DMA_FLAG_TEIF | \
																					sEE_TX_DMA_FLAG_HTIF | sEE_TX_DMA_FLAG_TCIF);
	/* Disable the EE I2C Tx DMA stream */
	DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
	/* Configure the DMA stream for the EE I2C peripheral TX direction */
	DMA_DeInit(sEE_I2C_DMA_STREAM_TX);
	sEEDMA_InitStructure.DMA_Channel = sEE_I2C_DMA_CHANNEL;
	sEEDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)sEE_I2C_DR_Address;
	sEEDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;    /* This parameter will be configured durig communication */;
	sEEDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; /* This parameter will be configured durig communication */
	sEEDMA_InitStructure.DMA_BufferSize = 0xFFFF;              /* This parameter will be configured durig communication */
	sEEDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	sEEDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	sEEDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	sEEDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	sEEDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	sEEDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	sEEDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	sEEDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	sEEDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	sEEDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(sEE_I2C_DMA_STREAM_TX, &sEEDMA_InitStructure);
	 
	/* Clear any pending flag on Rx Stream */
	DMA_ClearFlag(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_FEIF | sEE_RX_DMA_FLAG_DMEIF | sEE_RX_DMA_FLAG_TEIF | \
																				 sEE_RX_DMA_FLAG_HTIF | sEE_RX_DMA_FLAG_TCIF);
	/* Disable the EE I2C DMA Rx stream */
	DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
	/* Configure the DMA stream for the EE I2C peripheral RX direction */
	DMA_DeInit(sEE_I2C_DMA_STREAM_RX);
	DMA_Init(sEE_I2C_DMA_STREAM_RX, &sEEDMA_InitStructure);
		
	/* Enable the DMA Channels Interrupts */
	DMA_ITConfig(sEE_I2C_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
	DMA_ITConfig(sEE_I2C_DMA_STREAM_RX, DMA_IT_TC, ENABLE);      
}
/*********************************************************************************************************
*	函 数 名: sEE_LowLevel_DMAConfig
*	功能说明: 初始化DMA
*	形    参: pBuffer->存储地址，BufferSize->DMA数据个数,Direction->方向
*	返 回 值: 无
*********************************************************************************************************/
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{ 
 /* Initialize the DMA with the new parameters */
	if (Direction == sEE_DIRECTION_TX)
	{
		sEEDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pBuffer;
		sEEDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;    
		sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;  
		DMA_Init(sEE_I2C_DMA_STREAM_TX, &sEEDMA_InitStructure);  
	}
	else
	{ 
		/* Configure the DMA Rx Stream with the buffer address and the buffer size */
		sEEDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pBuffer;
		sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;      
		DMA_Init(sEE_I2C_DMA_STREAM_RX, &sEEDMA_InitStructure);    
	}
}


///*********************************************************************************************************
//*	函 数 名: sEE_ReadBuffer
//*	功能说明: 从FLASH读取数据
//*	形    参: pBuffer->存放读取得到的数据,ReadAddr->FLASH内部地址,NumByteToRead->
//*	返 回 值: 无
//*********************************************************************************************************/
//uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
//{  
///* Set the pointer to the Number of data to be read. This pointer will be used 
//       by the DMA Transfer Completer interrupt Handler in order to reset the 
//       variable to 0. User should check on this variable in order to know if the 
//       DMA transfer has been complete or not. */
//   sEEDataReadPointer = NumByteToRead;
//   
//   /*!< While the bus is busy */
//   sEETimeout = sEE_LONG_TIMEOUT;
//   while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
//   {
//     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//   }
//   
//   /*!< Send START condition */
//   I2C_GenerateSTART(sEE_I2C, ENABLE);
//   
//   /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
//	 /* 主机模式选择，产生Start信号之后，必须等待EV5置位，读取SR寄存器，自动清除该标志*/
//   sEETimeout = sEE_FLAG_TIMEOUT;
//   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
//   {
//     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//   }
//   
//   /*!< Send EEPROM address for write */
//	 /* 发送要读取EEPROM的地址 */
//   I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
// 
//   /*!< Test on EV6 and clear it */
//	 /* 主机模式，等待EV6置位*/
//   sEETimeout = sEE_FLAG_TIMEOUT;
//   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//   {
//     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//   } 
// 
// #ifdef sEE_M24C08  
//   
//   /*!< Send the EEPROM's internal address to read from: Only one byte address */
//   I2C_SendData(sEE_I2C, ReadAddr);  
// 
// #elif defined (sEE_M24C64_32)
// 
//   /*!< Send the EEPROM's internal address to read from: MSB of the address first */
//   I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));    
// 
//   /*!< Test on EV8 and clear it */
//		/* 一个数据正在发送中，需等待数据发送完成，EV8_1指示SR和DR都为空*/
//   sEETimeout = sEE_FLAG_TIMEOUT;
//   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
//  {
//     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//   }
// 
//   /*!< Send the EEPROM's internal address to read from: LSB of the address */
//   I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));    
//   
// #endif /*!< sEE_M24C08 */
// 
//   /*!< Test on EV8 and clear it */
//	 /* 等待最后一个数据发送完成，查询I2C_FLAG_BTF位*/
//   sEETimeout = sEE_FLAG_TIMEOUT;
//   while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
//   {
//     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//   }
//   
//   /*!< Send STRAT condition a second time */  
//   I2C_GenerateSTART(sEE_I2C, ENABLE);
//   
//   /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
//   sEETimeout = sEE_FLAG_TIMEOUT;
//   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
//   {
//     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//   } 
//   
//   /*!< Send EEPROM address for read */
//   I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Receiver);  
//   
//   /* If number of data to be read is 1, then DMA couldn't be used */
//   /* One Byte Master Reception procedure (POLLING) ---------------------------*/
//   if ((uint16_t)(*NumByteToRead) < 2)
//   {
//     /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
//     sEETimeout = sEE_FLAG_TIMEOUT;
//     while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_ADDR) == RESET)
//     {
//       if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//     }     
//     
//     /*!< Disable Acknowledgment */
//     I2C_AcknowledgeConfig(sEE_I2C, DISABLE);   
//     
//     /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
//     (void)sEE_I2C->SR2;
//     
//     /*!< Send STOP Condition */
//     I2C_GenerateSTOP(sEE_I2C, ENABLE);
//     
//     /* Wait for the byte to be received */
//     sEETimeout = sEE_FLAG_TIMEOUT;
//     while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
//     {
//       if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//     }
//     
//     /*!< Read the byte received from the EEPROM */
//     *pBuffer = I2C_ReceiveData(sEE_I2C);
//     
//     /*!< Decrement the read bytes counter */
//     (uint16_t)(*NumByteToRead)--;        
//     
//     /* Wait to make sure that STOP control bit has been cleared */
//     sEETimeout = sEE_FLAG_TIMEOUT;
//     while(sEE_I2C->CR1 & I2C_CR1_STOP)
//     {
//       if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//     }  
//     
//     /*!< Re-Enable Acknowledgment to be ready for another reception */
//     I2C_AcknowledgeConfig(sEE_I2C, ENABLE);    
//   }
//   else/* More than one Byte Master Reception procedure (DMA) -----------------*/
//   {
//     /*!< Test on EV6 and clear it */
//     sEETimeout = sEE_FLAG_TIMEOUT;
//     while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//     {
//       if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
//     }  
//     
//     /* Configure the DMA Rx Channel with the buffer address and the buffer size */
//     sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), sEE_DIRECTION_RX);
//     
//     /* Inform the DMA that the next End Of Transfer Signal will be the last one */
//     I2C_DMALastTransferCmd(sEE_I2C, ENABLE); 
//     
//     /* Enable the DMA Rx Stream */
//     DMA_Cmd(sEE_I2C_DMA_STREAM_RX, ENABLE);    
//   }
//   
//   /* If all operations OK, return sEE_OK (0) */
//   return sEE_OK;
//}

// uint32_t sEE_TIMEOUT_UserCallback(void)
// {
//   /* Block communication and all processes */
//	 I2C_SoftwareResetCmd(sEE_I2C,ENABLE);
////   while (1)
////   {   
////		 
////   }
//	 return 0;
// }
/*********************************************************************************************************
*	函 数 名: sEE_WritePage
*	功能说明: 写数据到EEPROM
*	形    参: pBuffer->需要写入EEPORM的数据,WriteAddr->FLASH内部地址,NumByteToWrite->
*	返 回 值: 正确写入，返回1
*********************************************************************************************************/
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
 { 
   /* Set the pointer to the Number of data to be written. This pointer will be used 
       by the DMA Transfer Completer interrupt Handler in order to reset the 
       variable to 0. User should check on this variable in order to know if the 
       DMA transfer has been complete or not. */
   sEEDataWritePointer = NumByteToWrite;  
   
   /*!< While the bus is busy */
   sEETimeout = sEE_LONG_TIMEOUT;
   while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
   {
     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
   }
   
   /*!< Send START condition */
   I2C_GenerateSTART(sEE_I2C, ENABLE);
   
   /*!< Test on EV5 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
   {
     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
   }
   
   /*!< Send EEPROM address for write */
   sEETimeout = sEE_FLAG_TIMEOUT;
   I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
 
   /*!< Test on EV6 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
   {
     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
   }
 
 #ifdef sEE_M24C08
   
   /*!< Send the EEPROM's internal address to write to : only one byte Address */
   I2C_SendData(sEE_I2C, WriteAddr);
   
 #elif defined(sEE_M24C64_32)
   
   /*!< Send the EEPROM's internal address to write to : MSB of the address first */
   I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));
 
   /*!< Test on EV8 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT;  
   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
   {
     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
   }  
   
   /*!< Send the EEPROM's internal address to write to : LSB of the address */
   I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
   
 #endif /*!< sEE_M24C08 */  
   
   /*!< Test on EV8 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT; 
   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
   {
     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
   }  
   
   /* Configure the DMA Tx Channel with the buffer address and the buffer size */
   sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), sEE_DIRECTION_TX);
  
   /* Enable the DMA Tx Stream */
   DMA_Cmd(sEE_I2C_DMA_STREAM_TX, ENABLE);
	 I2C_DMACmd(sEE_I2C, ENABLE);  
   /* If all operations OK, return sEE_OK (0) */
   return sEE_OK;
 }

/*********************************************************************************************************
*	函 数 名: sEE_WriteBuffer
*	功能说明: 按照页写入EEPROM
*	形    参: pBuffer->需要写入EEPORM的数据,WriteAddr->FLASH内部地址,NumByteToWrite->
*	返 回 值: 正确写入，返回1
*********************************************************************************************************/
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
   uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
   uint16_t Addr = 0;
	 /* 得到除开页之后的单地址数 ,如32bite页大小的EEPROM，3E(地址)=3E/32+3E%32 */
   Addr = WriteAddr % sEE_PAGESIZE;
	/* 距离页结束的个数，如3E地址，count=32-(3E/32+3E%32)=2*/
   count = sEE_PAGESIZE - Addr;
	/* 写入的整数页个数*/
   NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
	/* 除开整数页之后多余的个数*/
   NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
  
   /*!< If WriteAddr is sEE_PAGESIZE aligned  */
	/*	如果地址是页对齐 */
   if(Addr == 0) 
   {
     /*!< If NumByteToWrite < sEE_PAGESIZE */
		 /* 写入的数据个数不到一页 */
     if(NumOfPage == 0) 
     {
       /* Store the number of data to be written */
			 /* 实际需要写人的数据个数 */
       sEEDataNum = NumOfSingle;
       /* Start writing data */
       sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
       /* Wait transfer through DMA to be complete */
       sEETimeout = sEE_LONG_TIMEOUT;
       while (sEEDataNum > 0)
       {
         if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
       }
       sEE_WaitEepromStandbyState();
     }
     /*!< If NumByteToWrite > sEE_PAGESIZE */
     else  
     {
       while(NumOfPage--)
       {
         /* Store the number of data to be written */
				 /* 写入的个数大于一页，先写第一页*/
         sEEDataNum = sEE_PAGESIZE;        
         sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }      
         sEE_WaitEepromStandbyState();
				 /* 写的地址和写入数据的Buff都增加一个页*/
         WriteAddr +=  sEE_PAGESIZE;
         pBuffer += sEE_PAGESIZE;
				 /* 写完完整的页之后，写剩余的数据 */
       }
 
       if(NumOfSingle!=0)
       {
         /* Store the number of data to be written */
         sEEDataNum = NumOfSingle;          
         sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }    
         sEE_WaitEepromStandbyState();
       }
     }
   }
   /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
	 	/*	如果地址不是页对齐，首先先写完当前地址剩余的地址(也就是count个数据) */
   else 
   {
     /*!< If NumByteToWrite < sEE_PAGESIZE */
		 /* 第一种可能：umByteToWrite>count，先写完count(这个时候写完当前地址这一页)，然后接着写下一页*/
     if(NumOfPage== 0) 
     {
       /*!< If the number of data to be written is more than the remaining space 
       in the current page: */
       if (NumByteToWrite > count)
       {
         /* Store the number of data to be written */
         sEEDataNum = count;        
         /*!< Write the data contained in same page */
         sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }          
         sEE_WaitEepromStandbyState();      
         
         /* Store the number of data to be written */
         sEEDataNum = (NumByteToWrite - count);          
         /*!< Write the remaining data in the following page */
         sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }     
         sEE_WaitEepromStandbyState();        
       }      
       else      
       { /*	NumByteToWrite < count*/
				/* 第二种可能：直接按照顺序写*/				 
         /* Store the number of data to be written */
         sEEDataNum = NumOfSingle;         
         sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }          
         sEE_WaitEepromStandbyState();        
       }     
     }
     /*!< If NumByteToWrite > sEE_PAGESIZE */
     else
     {/*  NumByteToWrite > sEE_PAGESIZE 情况下*/
			 /* 都是一个原则，先写完当前地址后面到PAGE最高地址的count位 */
       NumByteToWrite -= count;
       NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
       NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
       
       if(count != 0)
       {  
         /* Store the number of data to be written */
         sEEDataNum = count;         
         sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }     
         sEE_WaitEepromStandbyState();
         WriteAddr += count;
         pBuffer += count;
       } 
       
       while(NumOfPage--)
       {
         /* Store the number of data to be written */
         sEEDataNum = sEE_PAGESIZE;          
         sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }        
         sEE_WaitEepromStandbyState();
         WriteAddr +=  sEE_PAGESIZE;
         pBuffer += sEE_PAGESIZE;  
       }
       if(NumOfSingle != 0)
       {
         /* Store the number of data to be written */
         sEEDataNum = NumOfSingle;           
         sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
         /* Wait transfer through DMA to be complete */
         sEETimeout = sEE_LONG_TIMEOUT;
         while (sEEDataNum > 0)
         {
           if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
         }         
         sEE_WaitEepromStandbyState();
       }
     }
   }  
 }
/*********************************************************************************************************
*	函 数 名: sEE_WaitEepromStandbyState
*	功能说明:等待和检查EEPROM是否完成了最后一次操作，最有可能用在写操作之后 
*	形    参: 空
*	返 回 值: 正确，返回1
*********************************************************************************************************/
uint32_t sEE_WaitEepromStandbyState(void)      
{
   __IO uint16_t tmpSR1 = 0;
   __IO uint32_t sEETrials = 0;
 
   /*!< While the bus is busy */
   sEETimeout = sEE_LONG_TIMEOUT;
   while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
   {
     if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
   }
 
   /* Keep looping till the slave acknowledge his address or maximum number 
      of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
      in stm322xg_eval_i2c_ee.h file) */
   while (1)
   {
     /*!< Send START condition */
     I2C_GenerateSTART(sEE_I2C, ENABLE);
 
     /*!< Test on EV5 and clear it */
     sEETimeout = sEE_FLAG_TIMEOUT;
     while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
     {
       if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
     }    
 
     /*!< Send EEPROM address for write */
     I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
     
     /* Wait for ADDR flag to be set (Slave acknowledged his address) */
     sEETimeout = sEE_LONG_TIMEOUT;
     do
     {     
       /* Get the current value of the SR1 register */
       tmpSR1 = sEE_I2C->SR1;
       
       /* Update the timeout value and exit if it reach 0 */
       if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
     }
     /* Keep looping till the Address is acknowledged or the AF flag is 
        set (address not acknowledged at time) */
     while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
      
     /* Check if the ADDR flag has been set */
     if (tmpSR1 & I2C_SR1_ADDR)
     {
       /* Clear ADDR Flag by reading SR1 then SR2 registers (SR1 have already 
          been read) */
       (void)sEE_I2C->SR2;
       
       /*!< STOP condition */    
       I2C_GenerateSTOP(sEE_I2C, ENABLE);
         
       /* Exit the function */
       return sEE_OK;
     }
     else
     {
       /*!< Clear AF flag */
       I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);                  
     }
     
     /* Check if the maximum allowed number of trials has bee reached */
     if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
     {
       /* If the maximum number of trials has been reached, exit the function */
       return sEE_TIMEOUT_UserCallback();
     }
   }
 }
/*********************************************************************************************************
*	函 数 名: sEE_I2C_DMA_TX_IRQHandler
*	功能说明:DMA发送中断
*	形    参: 空
*	返 回 值: 空
*********************************************************************************************************/
void sEE_I2C_DMA_TX_IRQHandler(void)
{
   /* Check if the DMA transfer is complete */
   if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF) != RESET)
   {  
     /* Disable the DMA Tx Stream and Clear TC flag */  
     DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
     DMA_ClearFlag(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF);
 
     /*!< Wait till all data have been physically transferred on the bus */
     sEETimeout = sEE_LONG_TIMEOUT;
     while(!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF))
     {
       if((sEETimeout--) == 0) sEE_TIMEOUT_UserCallback();
     }
     
     /*!< Send STOP condition */
     I2C_GenerateSTOP(sEE_I2C, ENABLE);
     
     /* Reset the variable holding the number of data to be written */
     *sEEDataWritePointer = 0;  
   }
}
/*********************************************************************************************************
*	函 数 名: sEE_I2C_DMA_RX_IRQHandler
*	功能说明:DMA接收中断
*	形    参: 空
*	返 回 值: 空
*********************************************************************************************************/
//void sEE_I2C_DMA_RX_IRQHandler(void)
//{
//   /* Check if the DMA transfer is complete */
//   if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF) != RESET)
//   {      
//     /*!< Send STOP Condition */
//     I2C_GenerateSTOP(sEE_I2C, ENABLE);    
//     
//     /* Disable the DMA Rx Stream and Clear TC Flag */  
//     DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
//     DMA_ClearFlag(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF);
//     
//     /* Reset the variable holding the number of data to be read */
//     *sEEDataReadPointer = 0;
//   }
//} 

/*********************************************************************************************************
*	函 数 名: Buffercmp
*	功能说明: 比较写入和独处EEPROM的数据是否相等
*	形    参: pBuffer->常量,pBuffer1->FLASH读出的数据,BufferLength数据长度
*	返 回 值: 无
*********************************************************************************************************/
TestStatus Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
 {
   while(BufferLength--)
   {
     if(*pBuffer != *pBuffer1)
     {
       return FAILED;
     }
     
     pBuffer++;
     pBuffer1++;
   }
 
   return PASSED;  
 }

uint32_t sEE_TIMEOUT_UserCallback(void)
{

	return 0;
}
