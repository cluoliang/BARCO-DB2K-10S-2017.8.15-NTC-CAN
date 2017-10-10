/**
  ******************************************************************************
  * @file    stm32f2xx_it.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    06-June-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* Update the LocalTime by adding SYSTEMTICK_PERIOD_MS each SysTick interrupt */
	TimingDelay_Decrement();
	printf("aa\r\n");
}
/**
  * @brief  This function handles ethernet DMA interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
  
}

void I2Cx_ER_IRQHANDLER(void)
{
#if defined (I2C_SLAVE)
	if ((I2C_ReadRegister(I2Cx, I2C_Register_SR1) & 0xFF00) != 0x00)
	{
	/* Clears error flags */
		I2Cx->SR1 &= 0x00FF;
	}
	
#endif
}

void I2Cx_EV_IRQHANDLER(void)
{
	#if defined (I2C_SLAVE)
	Event = I2C_GetLastEvent(I2Cx);
	switch (Event)
	{
	/* Check on EV1 */
		case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:  
		I2C_SendData(I2Cx, TxBuffer[Tx_Idx++]);
		I2C_ITConfig(I2Cx, I2C_IT_BUF , ENABLE);
		break;
	/* Check on EV3 */
		case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
		case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:  		
			if (Tx_Idx < NumberOfByteToTransmit)
			{
				I2C_SendData(I2Cx, TxBuffer[Tx_Idx++]);
			}
			else
			{
			 /* Disable I2C event interrupt */
				I2C_ITConfig(I2Cx, I2C_IT_EVT  | I2C_IT_BUF, DISABLE);
			}
		break;
		/* check on EV1*/
		case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
		Rx_Idx = 0x00;
		break;
     /* Check on EV2*/
		case I2C_EVENT_SLAVE_BYTE_RECEIVED:  
		case (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_SR1_BTF):  
		RxBuffer[Rx_Idx++] = I2C_ReceiveData(I2Cx);
		break;
     /* Check on EV4 */
		case I2C_EVENT_SLAVE_STOP_DETECTED:             
		I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF);
		I2C_Cmd(I2Cx, ENABLE);
		break;
		default:
		break;
	}
	#endif

}
#ifdef USE_CAN1
void CAN1_RX0_IRQHandler(void)
{
	static  unsigned char RecieveBuff[8]={0xff,0xee,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
	static CanRxMsg RxMessage;
	uint8_t i = 0;
	RxMessage.StdId = 0x00;
	RxMessage.ExtId = 0x00;
	RxMessage.IDE = CAN_ID_STD;
	RxMessage.DLC = 0;
	RxMessage.FMI = 0;
	for (i = 0;i < 8;i++)
	{
		RxMessage.Data[i] = 0x00;
	}
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(!(memcmp((void*)RecieveBuff,(void*)RxMessage.Data,8))&&(RxMessage.StdId==0x018))
	{	
		Can_Recieve_Flag=Receive_Success;

		Init_TxMes(&TxMessage);

//		printf("SU\r\n");

	}
	
}
#endif
