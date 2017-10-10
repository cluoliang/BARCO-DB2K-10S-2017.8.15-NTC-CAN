/***********************************************************************
文件名称：DS18B20.H
功    能：完成对对DS18B20的基本操作
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
#ifndef _DS18B20_H
#define _DS18B20_H

extern uint8_t g_rom_id[8];
extern uint8_t Temp_Hex;

#define MAXNUM  4
#define Upper_Limit	70
#define Lower_Limit	60

extern uint8_t ID_Buff[MAXNUM][8];

//#define USR_CAN
#define HIGH  1
#define LOW   0

#define DS18B20_CLK     RCC_AHB1Periph_GPIOB
#define DS18B20_PIN     GPIO_Pin_0                  
#define DS18B20_PORT		GPIOB 


#define DS18B20_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define  DS18B20_DATA_IN()	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

void DS18B20_GPIO_Configuration(void);
static void DS18B20_Mode_IPU(void);
static void DS18B20_Mode_Out_PP(void);					
static void  DS18B20_Reset(void);
static uint8_t DS18B20_Presence(void);
static uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Read_Byte(void);
uint8_t DS18B20_Read2Bit(void) ;
void DS18B20_Write_Byte(uint8_t dat);
void DS18B20_Write_Bit(uint8_t dat);
void DS18B20_Start(void);
uint8_t DS18B20_Init(void);
float DS18B20_ReadTempReg(void);
uint8_t DS18B20_ReadID(uint8_t *_id);
float DS18B20_ReadTempByID(uint8_t *_id);
unsigned char Read_ROM_ID(unsigned char * data);
uint8_t DS18B20_SearchROM(u8 (*pID)[8],u8 Num);
float DS18B20_ReadDesignateTemper(uint8_t pID[8]);
//uint8_t CheckCrc(void);
#endif
