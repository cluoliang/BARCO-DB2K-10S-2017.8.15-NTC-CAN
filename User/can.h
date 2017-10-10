#ifndef _CAN_H
#define _CAN_H
#include "main.h"

#define USE_CAN1
/* #define USE_CAN2 */
#ifdef  USE_CAN1
#define CANx CAN1
#else /* USE_CAN2 */
#define CANx CAN2
#endif  /* USE_CAN1 */

#define CAN_CLK   (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
#define CAN_RX_PIN   GPIO_Pin_0
#define CAN_TX_PIN   GPIO_Pin_1
#define CAN_GPIO_PORT   GPIOD
#define CAN_GPIO_CLK   RCC_AHB1Periph_GPIOD
#define CAN_AF_PORT   GPIO_AF_CAN1
#define CAN_RX_SOURCE   GPIO_PinSource0
#define CAN_TX_SOURCE   GPIO_PinSource1
#define Receive_Success 0x01
#define Clear_Flag 0x00

#define Laser_ON 0x02
#define Laser_OFF 0x01
#define Mode_2D 0x02
#define Mode_3D 0x03
#define Switch_Normal 0x02
#define Switch_Annormal 0x01
#define Light_Normal 0x02
#define Light_Annormal 0x01

extern CanTxMsg TxMessage;

typedef struct{
	uint8_t Laser_On_Off;
	uint8_t Display_Mode;
	uint8_t Exter_Switch;
	uint8_t Ambient_Light;
	uint8_t Optical_Temp;
	uint8_t Ambient_Temp;
}CAN_Trans_Byte;

extern volatile CAN_Trans_Byte CAN_COMMAND;
extern uint8_t Can_Recieve_Flag;
void CAN_Config(void);
uint8_t Manage_Can_Receive(uint8_t Can_Recieve_Flag);
void Init_TxMes(CanTxMsg *TxMessage);
#endif
