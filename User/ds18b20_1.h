/***********************************************************************
文件名称：DS18B20.H
功    能：完成对对DS18B20的基本操作
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
#ifndef _DS18B20_1_H
#define _DS18B20_1_H

extern uint8_t g_rom_id_1[8];
extern uint8_t Temp_Hex_1;

#define MAXNUM_1  2
#define Upper_Limit	70
#define Lower_Limit	60

extern uint8_t ID_Buff_1[MAXNUM][8];

//#define USR_CAN
#define HIGH  1
#define LOW   0

#define DS18B20_CLK_1     RCC_AHB1Periph_GPIOB
#define DS18B20_PIN_1     GPIO_Pin_1                  
#define DS18B20_PORT		GPIOB 


#define DS18B20_DATA_OUT_1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define  DS18B20_DATA_IN_1()	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

void DS18B20_GPIO_Configuration_1(void);
static void DS18B20_Mode_IPU_1(void);
static void DS18B20_Mode_Out_PP_1(void);					
static void  DS18B20_Reset_1(void);
static uint8_t DS18B20_Presence_1(void);
static uint8_t DS18B20_Read_Bit_1(void);
uint8_t DS18B20_Read_Byte_1(void);
uint8_t DS18B20_Read2Bit_1(void) ;
void DS18B20_Write_Byte_1(uint8_t dat_1);
void DS18B20_Write_Bit_1(uint8_t dat_1);
void DS18B20_Start_1(void);
uint8_t DS18B20_Init_1(void);
float DS18B20_ReadTempReg_1(void);
uint8_t DS18B20_ReadID_1(uint8_t *_id);
float DS18B20_ReadTempByID_1(uint8_t *_id);
unsigned char Read_ROM_ID_1(unsigned char * dat_1a);
uint8_t DS18B20_SearchROM_1(u8 (*pID)[8],u8 Num);
float DS18B20_ReadDesignateTemper_1(uint8_t pID[8]);
//uint8_t CheckCrc_1(void);
#endif
