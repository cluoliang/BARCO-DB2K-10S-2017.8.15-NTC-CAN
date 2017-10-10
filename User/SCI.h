/***********************************************************************
�ļ����ƣ�SCI.h
��    �ܣ�
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
***********************************************************************/
#ifndef  _SCI_H_
#define  _SCI_H_

#define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_4)	//����ʹ��
#define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_4)  	//����ʹ��
#define RS485_REC_BUFF_SIZE				100
#define RS232_REC_BUFF_SIZE				100
//#define RS485_END_FLAG1	'?'			//RS485һ�����ݽ�����־1 
//#define RS485_END_FLAG2	';'			//RS485һ�����ݽ�����־2 
//#define RS232_END_FLAG1	'?'			//RS232һ�����ݽ�����־1 
//#define RS232_END_FLAG2	';'			//RS232һ�����ݽ�����־2 

#define RS232_START_FLAG1	0xFE		//RS232һ�����ݽ�����־1 
#define RS232_END_FLAG2	0xFF			//RS232һ�����ݽ�����־2 

extern volatile unsigned char RS232_REC_Flag ;
extern volatile unsigned char RS485_REC_Flag ;
extern volatile unsigned char RS232_buff[RS232_REC_BUFF_SIZE] ;//���ڽ�������
extern volatile unsigned char RS485_buff[RS485_REC_BUFF_SIZE] ;//���ڽ�������
extern volatile unsigned int RS232_rec_counter ;//����RS232���ռ���
extern volatile unsigned int RS485_rec_counter ;//����RS485���ռ���

void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
void RS485_Send_Data(unsigned char *send_buff,unsigned int length); 
void RS485_Delay(uint32_t nCount);
void USART1_Configuration(int baudrate);
void RS232_Send_Byte( char Send_Byte);

#endif