#ifndef  _UART_H_
#define  _UART_H_


#define RS232_REC_BUFF_SIZE				100
#define RS232_START_FLAG1	0xFE		//RS232一桢数据结束标志1 
#define RS232_END_FLAG2	0xFF			//RS232一桢数据结束标志2 


extern volatile unsigned char RS232_buff[RS232_REC_BUFF_SIZE] ;//用于接收数据
extern volatile unsigned int RS232_rec_counter ;//用于RS232接收计数
extern volatile unsigned char packerflag;//是否接受到完整的数据包
extern volatile unsigned char Uart_Size;

extern volatile unsigned char StaBy_Flag; /*待机状态字*/
extern volatile unsigned char count;
extern volatile unsigned char Start_Flag;/*开灯状态字*/
extern volatile unsigned char Stop_Flag;/*关灯状态字*/


void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
void RS485_Send_Data(unsigned char *send_buff,unsigned int length);
static void RS485_Delay(uint32_t nCount);
void USART_Configuration(int baudrate);
unsigned char RS232_Accept_byte(void);
void RS232_Send_Byte( char Send_Byte);

#endif
