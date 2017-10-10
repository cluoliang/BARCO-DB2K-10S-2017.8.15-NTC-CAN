
#include "main.h"

uint16_t Channel_0, Channel_1;
uint8_t ReadBuffer = 0;
uint32_t lux;
void TSL2581_INIT_GPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	

}


void TSL2581_Interrupt_Threshold(uint16_t min,uint16_t max)
{
	SET_Interrupt_Threshold(min,max);
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)==1)
	{
		printf("interrupt = 1\r\n");

	}else{		
		Reload_register();
		printf("interrupt = 0\r\n");
	}
}
/*******************************************************************************
* Function Name  : I2C_AcknowledgePolling
* Description    : 等待获取I2C总线控制权 判断忙状态
* Input          : - I2Cx:I2C寄存器基址
*                  - I2C_Addr:从器件地址
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_AcknowledgePolling(I2C_TypeDef* I2C , uint8_t I2C_Addr)
{
  vu16 SR1_Tmp;
  do
  {   
    I2C_GenerateSTART(I2C, ENABLE); /*起始位*/
    /*读SR1*/
    SR1_Tmp = I2C_ReadRegister(I2C, I2C_Register_SR1);
    /*器件地址(写)*/

	I2C_Send7bitAddress(I2C, I2C_Addr, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(I2C, I2C_Register_SR1) & 0x0002));
  
  I2C_ClearFlag(I2C, I2C_FLAG_AF);
    
  I2C_GenerateSTOP(I2C, ENABLE);  /*停止位*/  
}

/*********************************************************************************************************
*	函 数 名: I2C_DEV_Write
*	功能说明: 写命令到tsl2581
*	形    参: I2C_Addr->tsl2581的地址,Register_Addr->tsl2581寄存器地址,Register_Data->命令数据
*	返 回 值: 无
*********************************************************************************************************/
uint8_t I2C_DEV_Write(uint16_t I2C_Addr,uint16_t Register_Addr,uint8_t Register_Data)
{
	 
   /*!< Send START condition */
   I2C_GenerateSTART(I2C1, ENABLE);
   
   /*!< Test on EV5 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
   {
     if((sEETimeout--) == 0) return I2C_Mode_Select;
   }
   
   /*!< Send EEPROM address for write */
   I2C_Send7bitAddress(I2C1, I2C_Addr, I2C_Direction_Transmitter);
 
   /*!< Test on EV6 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
   {
     if((sEETimeout--) == 0) return I2C_Trans_Addr;
   }
   
   /*!< Send the EEPROM's internal address to write to : only one byte Address */
   I2C_SendData(I2C1, Register_Addr);   
   /*!< Test on EV8 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT;  	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
   {
     if((sEETimeout--) == 0) return I2C_Trans_Byte;
   }

   I2C_SendData(I2C1, Register_Data);   
   /*!< Test on EV8 and clear it */
   sEETimeout = sEE_FLAG_TIMEOUT;  	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
   {
     if((sEETimeout--) == 0) return I2C_Trans_Byte;
   }
    I2C_GenerateSTOP(I2C1, ENABLE);	
	  I2C_AcknowledgePolling(I2Cx,I2C_Addr);
		Stop(2);
	 return I2C_Write_Success;
}

uint8_t I2C_DEV_Read(uint16_t Register_Addr)
{
	uint8_t ReadBuffer;
	I2C_Mem_Read(ADDR_FLOAT_Read,Register_Addr,&ReadBuffer,1);
//	printf("Flag=%d\r\n",Flag);
	return ReadBuffer;
}

/*********************************************************************************************************
*	函 数 名: I2C_Mem_Read
*	功能说明: 写命令到tsl2581
*	形    参: I2C_Addr->tsl2581的地址,Register_Addr->tsl2581寄存器地址,ReadBuffer->存放获取到的数据
*	返 回 值: 无
*********************************************************************************************************/
uint8_t I2C_Mem_Read(uint16_t I2C_Addr,uint16_t Register_Addr,uint8_t *ReadBuffer,uint8_t Size)
{
		if(Size==0)
		return 1;
		
   I2C_AcknowledgeConfig(I2C1, ENABLE);
		
   /*!< While the bus is busy */
   sEETimeout = sEE_LONG_TIMEOUT;
   while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
   {
     if((sEETimeout--) == 0) return I2C_Busy;
   }
   /*!< Send START condition */
   I2C_GenerateSTART(I2C1, ENABLE);

   /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	 /* 主机模式选择，产生Start信号之后，必须等待EV5置位，读取SR寄存器，自动清除该标志*/
   sEETimeout = sEE_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
   {
     if((sEETimeout--) == 0) return I2C_Mode_Select;
   }
   
   /*!< Send EEPROM address for write */
	 /* 发送要读取EEPROM的地址 */
   I2C_Send7bitAddress(I2C1, I2C_Addr, I2C_Direction_Transmitter);
//	 printf("a\r\n");
   /*!< Test on EV6 and clear it */
	 /* 主机模式，等待EV6置位*/
   sEETimeout = sEE_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
   {
     if((sEETimeout--) == 0) return I2C_Trans_Byte;
   } 
   I2C_SendData(I2C1, Register_Addr); 
//	 	 printf("b\r\n");
	sEETimeout = sEE_FLAG_TIMEOUT;
   while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
   {
     if((sEETimeout--) == 0) return I2C_Start;
   }

   /*!< Send STRAT condition a second time */  
   I2C_GenerateSTART(I2C1, ENABLE);	 
   sEETimeout = sEE_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
   {
     if((sEETimeout--) == 0) return I2C_Mode_Select;
   } 	 
   I2C_Send7bitAddress(I2C1, I2C_Addr, I2C_Direction_Receiver); 	
// printf("c\r\n");
   if (Size == 1)
   {
     /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
     sEETimeout = sEE_FLAG_TIMEOUT;
     while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_ADDR) == RESET)
     {
       if((sEETimeout--) == 0) return 0;
     }     
     
     /*!< Disable Acknowledgment */
     I2C_AcknowledgeConfig(sEE_I2C, DISABLE);   
     
     /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
     (void)sEE_I2C->SR2;
     
     /*!< Send STOP Condition */
     I2C_GenerateSTOP(sEE_I2C, ENABLE);
     
     /* Wait for the byte to be received */
     sEETimeout = sEE_FLAG_TIMEOUT;
     while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
     {
       if((sEETimeout--) == 0) return 0;
     }
     
     /*!< Read the byte received from the EEPROM */
     *ReadBuffer = I2C_ReceiveData(sEE_I2C);
     /*!< Decrement the read bytes counter */
     Size--;        
     
     /* Wait to make sure that STOP control bit has been cleared */
     sEETimeout = sEE_FLAG_TIMEOUT;
     while(sEE_I2C->CR1 & I2C_CR1_STOP)
     {
       if((sEETimeout--) == 0) return 0;
     }  
     
     /*!< Re-Enable Acknowledgment to be ready for another reception */
     I2C_AcknowledgeConfig(sEE_I2C, ENABLE);    
   }
	  return *ReadBuffer;
 }
 
///**********************************************************************************************
//* @brief  I2C_DEV_Write(uint16_t Register_Addr,uint8_t Register_Data)
//* @param  Register_Addr : Register Address	
//* @param  Register_Data : Register data
//* @param  return Write state,Write success is 1,Write fail is 0
//**********************************************************************************************/
//uint8_t I2C_DEV_Write(uint16_t I2C_Addr,uint16_t Register_Addr,uint8_t Register_Data)
//{
//	if(HAL_I2C_Mem_Write(&hi2c1, I2C_Addr, Register_Addr, I2C_MEMADD_SIZE_8BIT, &Register_Data, 1, 1000) == HAL_OK)
//	{
//		return Write_success;
//	} else{
//		return Write_fail;
//	}
//}

///**********************************************************************************************
//* @brief  I2C_DEV_Read(uint16_t Register_Addr)
//* @param  Register_Addr : Register Address	
//* @param  
//* @param  return read data (uchar)
//**********************************************************************************************/
//uint8_t I2C_DEV_Read(uint16_t Register_Addr)
//{
//	uint8_t ReadBuffer[1];
//	HAL_I2C_Mem_Read(&hi2c1, ADDR_FLOAT_Read, Register_Addr, I2C_MEMADD_SIZE_8BIT,ReadBuffer,1, 1000);
//	return ReadBuffer[0];
//}
 
/**********************************************************************************************
* @brief  	I2C_DEV_init()
* @param   	power on ,set gain is 16,interrupt is 402ms
* @param   
* @param    This field selects the integration time for each conversion.
**********************************************************************************************/
void I2C_DEV_init(void)
{ 
	/* write date from tsl2561 */
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | CONTROL,CONTROL_POWERON);//power on
	Stop(20);//Wait 2 seconds for power on
	
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | TIMING, INTEGRATIONTIME_400MS);  //400MS
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | CONTROL, ADC_EN | CONTROL_POWERON); //Every ADC cycle generates interrupt
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | INTERRUPT, INTR_INTER_MODE);	//TEST MODE
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | ANALOG, GAIN_1X);				//GAIN = 16
}

/**********************************************************************************************
* @brief  	Reload_register()
* @param   Interrupts need to be maintained for several cycles
* @param   When the interrupt bit is 0, reload the register
* @param   Configure the special registers, clear the interrupt bits, and then re-enable the ADC
***********************************************************************************************/
void Reload_register(void)
{
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | TRANSACTION_SPECIAL | SPECIAL_FUN_INTCLEAR, INTR_INTER_MODE);
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | CONTROL, ADC_EN | CONTROL_POWERON); //Every ADC cycle generates interrupt
}

/**********************************************************************************************
* @brief  	SET_Interrupt_Threshold(uint32_t low,uint32_t high)
* @param   	low and high max 2^16 = 65536
* @param   
* @param    This field selects the integration time for each conversion.
**********************************************************************************************/
void SET_Interrupt_Threshold(uint16_t min,uint16_t max)
{
	uint8_t DataLLow,DataLHigh,DataHLow,DataHHigh;
	DataLLow = min % 256;
	DataLHigh = min / 256;
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | THLLOW, DataLLow);  
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | THLHIGH, DataLHigh);  

	DataHLow = max % 256;
	DataHHigh = max / 256;
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | THHLOW, DataHLow);  
	I2C_DEV_Write(ADDR_FLOAT_Write,COMMAND_CMD | THHHIGH, DataHHigh); 
}	

/**********************************************************************************************
* @brief  	Read_Channel()
* @param    
* @param   	read two ADC data
* @param     
**********************************************************************************************/
void Read_Channel()
{	
	uint8_t DataLow,DataHigh;
	DataLow = I2C_DEV_Read(COMMAND_CMD | TRANSACTION | DATA0LOW);
	DataHigh = I2C_DEV_Read(COMMAND_CMD | TRANSACTION | DATA0HIGH);
	Channel_0 = 256 * DataHigh + DataLow ;
	DataLow = I2C_DEV_Read(COMMAND_CMD | TRANSACTION | DATA1LOW);
	DataHigh = I2C_DEV_Read(COMMAND_CMD | TRANSACTION | DATA1HIGH);
	Channel_1 = 256 * DataHigh + DataLow ;
	
//	printf("Channel_0 = %d\n",Channel_0);
//	printf("Channel_1 = %d\n",Channel_1);
}

/**********************************************************************************************
* @brief  	calculateLux()
* @param    Channel_0 and Channel_1 is for TSL2561_Read_Channel();
* @param   	// Arguments: unsigned int iGain - gain, where 0:1X, 1:8X, 2:16X, 3:128X
* @param   	// unsigned int tIntCycles - INTEG_CYCLES defined in Timing Register
**********************************************************************************************/
uint32_t calculateLux(uint16_t iGain,uint16_t tIntCycles)
{
  unsigned long chScale0;
  unsigned long chScale1;
  unsigned long channel1;
  unsigned long channel0;
  unsigned long temp;
  unsigned long ratio1 = 0;
  unsigned long ratio;
  unsigned long lux_temp;
  unsigned int b, m;

  // No scaling if nominal integration (148 cycles or 400 ms) is used
  if (tIntCycles == NOM_INTEG_CYCLE)
  {
//     chScale0 = 65536;
	  chScale0 = (1 << (CH_SCALE));
  }
  else
    chScale0 = (NOM_INTEG_CYCLE << CH_SCALE) / tIntCycles;
  switch (iGain)
  {
    case 0: // 1x gain
      chScale1 = chScale0; // No scale. Nominal setting
      break;
    case 1: // 8x gain
      chScale0 = chScale0 >> 3; // Scale/multiply value by 1/8
      chScale1 = chScale0;
      break;
    case 2: // 16x gain
      chScale0 = chScale0 >> 4; // Scale/multiply value by 1/16
      chScale1 = chScale0;
      break;
    case 3: // 128x gain
      chScale1 = chScale0 / CH1GAIN128X;
      chScale0 = chScale0 / CH0GAIN128X;
      break;
  }
  // Read Channel for ADC
	
  Read_Channel();
  // scale the channel values
  channel0 = (Channel_0 * chScale0) >>  CH_SCALE;
  channel1 = (Channel_1 * chScale1) >>  CH_SCALE;

  // find the ratio of the channel values (Channel1/Channel0)
  if (channel0 != 0)
    ratio1 = (channel1 << (RATIO_SCALE + 1)) / channel0;
  ratio = (ratio1 + 1) >> 1;	  									 // round the ratio value

  if ((ratio >= 0) && (ratio <= K1C))
  {    b = B1C;    m = M1C;  }
  else if (ratio <= K2C)
  {    b = B2C;    m = M2C;  }
  else if (ratio <= K3C)
  {    b = B3C;    m = M3C;  }
  else if (ratio <= K4C)//276
  {    b = B4C;    m = M4C;  }
  else if (ratio > K5C)//276
  {    b = B5C;    m = M5C;  }

  temp = ((channel0 * b) - (channel1 * m));
  temp += (1 << (LUX_SCALE - 1));			// round lsb (2^(LUX_SCALE-1))
//  temp = temp + 32768;
  lux_temp = temp >> LUX_SCALE;			// strip off fractional portion
  return (lux_temp);		  							// Signal I2C had no errors
}

void Tsl2581_Init_Dev(void)
{
	I2C_DEV_Read(COMMAND_CMD | TRANSACTION |ID);
	I2C_DEV_init();
}

void Tsl2581_Basic_Init(void)
{
	I2C_GPIO_Config();
	I2C_Config();
//	TSL2581_INIT_GPIO();
}
