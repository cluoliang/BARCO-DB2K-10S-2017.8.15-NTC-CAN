#ifndef _I2C_H
#define _I2C_H

typedef enum
{
	I2C_Busy =0x00,
	I2C_Mode_Select =0x01,
	I2C_Trans_Addr =0x02,
	I2C_Trans_Byte =0x03,
	I2C_Write_Success =0x01,
	I2C_Start =0x04,
	
}I2C_Error_Statue;


#define  countof(a)   (sizeof(a) / sizeof(*(a))) 
#define  I2C_MASTER 
//#define I2C_SLAVE
#define  I2C_SPEED   100000 
#define  I2C_DUTYCYCLE   I2C_DutyCycle_2 
#define  SLAVE_ADDRESS   0x30 
#define  USER_TIMEOUT   ((uint32_t)0x64) 
#define  MASTER_MODE_TRANSMITTER   0x01 
#define  MASTER_MODE_RECEIVER   0x00 
#define  I2Cx   I2C1 
#define  I2Cx_CLK   RCC_APB1Periph_I2C1 
#define  I2Cx_EV_IRQn   I2C1_EV_IRQn 
#define  I2Cx_ER_IRQn   I2C1_ER_IRQn 
#define  I2Cx_EV_IRQHANDLER   I2C1_EV_IRQHandler 
#define  I2Cx_ER_IRQHANDLER   I2C1_ER_IRQHandler 
#define  I2Cx_SDA_GPIO_CLK   RCC_AHB1Periph_GPIOB 
#define  I2Cx_SDA_PIN   GPIO_Pin_7 
#define  I2Cx_SDA_GPIO_PORT   GPIOB 
#define  I2Cx_SDA_SOURCE   GPIO_PinSource7 
#define  I2Cx_SDA_AF   GPIO_AF_I2C1 
#define  I2Cx_SCL_GPIO_CLK   RCC_AHB1Periph_GPIOB 
#define  I2Cx_SCL_PIN   GPIO_Pin_6 
#define  I2Cx_SCL_GPIO_PORT   GPIOB 
#define  I2Cx_SCL_SOURCE   GPIO_PinSource6 
#define  I2Cx_SCL_AF   GPIO_AF_I2C1 
#define  TXBUFFERSIZE   (countof(TxBuffer) - 1) 
#define  RXBUFFERSIZE   TXBUFFERSIZE 


/*∂¡–¥Flash*/
#define sEE_OK   0
#define sEE_FAIL   1 
#define sEE_I2C   I2C1 
#define sEE_I2C_CLK   RCC_APB1Periph_I2C1 
#define sEE_I2C_SCL_PIN   GPIO_Pin_6 
#define sEE_I2C_SCL_GPIO_PORT   GPIOB 
#define sEE_I2C_SCL_GPIO_CLK   RCC_AHB1Periph_GPIOB
#define sEE_I2C_SCL_SOURCE   GPIO_PinSource6 
#define sEE_I2C_SCL_AF   GPIO_AF_I2C1 
#define sEE_I2C_SDA_PIN   GPIO_Pin_9
#define sEE_I2C_SDA_GPIO_PORT   GPIOB
#define sEE_I2C_SDA_GPIO_CLK   RCC_AHB1Periph_GPIOB
#define sEE_I2C_SDA_SOURCE   GPIO_PinSource9 
#define sEE_I2C_SDA_AF   GPIO_AF_I2C1 
#define sEE_I2C_DMA   DMA1 
#define sEE_I2C_DMA_CHANNEL   DMA_Channel_1 
#define sEE_I2C_DMA_STREAM_TX   DMA1_Stream6 
#define sEE_I2C_DMA_STREAM_RX   DMA1_Stream0 
#define sEE_I2C_DMA_CLK   RCC_AHB1Periph_DMA1 
#define sEE_I2C_DR_Address   ((uint32_t)0x40005410) 
#define sEE_I2C_DMA_TX_IRQn   DMA1_Stream6_IRQn 
#define sEE_I2C_DMA_RX_IRQn   DMA1_Stream0_IRQn
#define sEE_I2C_DMA_TX_IRQHandler   DMA1_Stream6_IRQHandler 
#define sEE_I2C_DMA_RX_IRQHandler   DMA1_Stream0_IRQHandler
#define sEE_I2C_DMA_PREPRIO   0 
#define sEE_I2C_DMA_SUBPRIO   0 
#define sEE_TX_DMA_FLAG_FEIF   DMA_FLAG_FEIF6 
#define sEE_TX_DMA_FLAG_DMEIF   DMA_FLAG_DMEIF6 
#define sEE_TX_DMA_FLAG_TEIF   DMA_FLAG_TEIF6 
#define sEE_TX_DMA_FLAG_HTIF   DMA_FLAG_HTIF6 
#define sEE_TX_DMA_FLAG_TCIF   DMA_FLAG_TCIF6
#define sEE_RX_DMA_FLAG_FEIF   DMA_FLAG_FEIF0 
#define sEE_RX_DMA_FLAG_DMEIF   DMA_FLAG_DMEIF0
#define sEE_RX_DMA_FLAG_TEIF   DMA_FLAG_TEIF0 
#define sEE_RX_DMA_FLAG_HTIF   DMA_FLAG_HTIF0 
#define sEE_RX_DMA_FLAG_TCIF   DMA_FLAG_TCIF0 
#define sEE_DIRECTION_TX   0 
#define sEE_DIRECTION_RX   1 
#define sEE_TIME_CONST   120 
#define I2C_SLAVE_ADDRESS7   0xA0 
#define sEE_HW_ADDRESS   0xA0
#define sEE_FLAG_TIMEOUT   ((uint32_t)0x100) 
#define sEE_LONG_TIMEOUT   ((uint32_t)(10 * sEE_FLAG_TIMEOUT)) 
#define sEE_MAX_TRIALS_NUMBER   300 
/* “≥∂¡°¢“≥–¥*/
#define sEE_PAGESIZE   32

/* ≥¨ ±±Í÷æ */
#define Bus_Timeout 1
#define Start_Timeout 1
#define Bus_Timeout 1
#define Start_Timeout 1

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus; 
extern volatile uint8_t TxBuffer[];
extern volatile uint8_t RxBuffer[];
extern volatile uint32_t  Event;
extern volatile uint8_t Tx_Idx;
extern volatile uint8_t Rx_Idx;
extern  uint8_t NumberOfByteToReceive;
extern  uint8_t NumberOfByteToTransmit;
extern volatile uint8_t MasterMode;
extern volatile uint16_t sEEAddress;
extern volatile uint16_t* sEEDataReadPointer;
extern volatile uint16_t* sEEDataReadPointer; 
extern volatile uint8_t *  sEEDataWritePointer ;
extern volatile uint32_t  sEETimeout; 
extern volatile uint8_t  sEEDataNum;

void I2C_Config(void);
void I2C_GPIO_Config(void);
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void);
uint32_t sEE_TIMEOUT_UserCallback(void);
uint32_t sEE_WaitEepromStandbyState(void);
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
TestStatus Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
#endif

