#ifndef  _UART_H_
#define  _UART_H_


#define RS232_REC_BUFF_SIZE				100
#define RS232_START_FLAG1	0xFE		//RS232һ�����ݽ�����־1 
#define RS232_END_FLAG2	0xFF			//RS232һ�����ݽ�����־2 


extern volatile unsigned char RS232_buff[RS232_REC_BUFF_SIZE] ;//���ڽ�������
extern volatile unsigned int RS232_rec_counter ;//����RS232���ռ���
extern volatile unsigned char packerflag;//�Ƿ���ܵ����������ݰ�
extern volatile unsigned char Uart_Size;

extern volatile unsigned char StaBy_Flag; /*����״̬��*/
extern volatile unsigned char count;
extern volatile unsigned char Start_Flag;/*����״̬��*/
extern volatile unsigned char Stop_Flag;/*�ص�״̬��*/


void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
void RS485_Send_Data(unsigned char *send_buff,unsigned int length);
static void RS485_Delay(uint32_t nCount);
void USART_Configuration(int baudrate);
unsigned char RS232_Accept_byte(void);
void RS232_Send_Byte( char Send_Byte);

#endif
