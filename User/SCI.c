
#include "main.H"
/***********************************************************************
文件名称：SCI.C
功    能：完成对usart1和usart2的操作
编写时间：2012.11.22
编 写 人：
注    意：本例程是通过判断两个特定的字符来确定一帧数据是否结束的。
USART1 时钟 : RCC_APB2PeriphClockCmd
USART1~6 时钟 :RCC_APB1PeriphClockCmd
***********************************************************************/

volatile unsigned char RS232_REC_Flag = 0;
volatile unsigned char RS485_REC_Flag = 0;
volatile  unsigned char RS232_buff[RS232_REC_BUFF_SIZE];//用于接收数据
volatile unsigned char RS485_buff[RS485_REC_BUFF_SIZE];//用于接收数据
volatile unsigned int RS232_rec_counter = 0;//用于RS232接收计数
volatile unsigned int RS485_rec_counter = 0;//用于RS485接收计数

volatile unsigned char RS232_buff_bite;//用于接收数单个字符
unsigned char recstatu;	//用于判定是否处于一个正在接受数据包的状态
volatile unsigned char packerflag;	//是否接受到完整的数据包
unsigned char Uart_Size=0;

volatile unsigned char StaBy_Flag = 0; /*待机状态字*/
volatile unsigned char count = 0;
volatile unsigned char Start_Flag = 0;/*开灯状态字*/
volatile unsigned char Stop_Flag = 0;/*关灯状态字*/

extern unsigned char Start_RXD_1[4];
extern unsigned char Stop_RXD_1[4];
extern unsigned char RXD_1[3];

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	return ch;
}

void USART1_Configuration(int baudrate)
{ 
	
	GPIO_InitTypeDef GPIO_InitStructure;	//定义GPIO_InitTypeDef类型的结构体成员GPIO_InitStructure
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能需要用到的GPIO管脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1 时钟
	
	USART_DeInit(USART1);	//复位串口1	
	USART_StructInit(&USART_InitStructure);	//载入默认USART参数
	USART_ClockStructInit(&USART_ClockInitStruct);	//载入默认USART参数 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //管脚PA9复用为USART1
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 	//配置串口1的管脚 PA8 USART1_EN PA9 USART1_TX PA10 USART1_RX  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;     
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                                                                                                 

	USART_ClockInit(USART1,&USART_ClockInitStruct);
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //接收中断使能
	USART_ClearITPendingBit(USART1, USART_IT_TC);	//清除中断TC位
	USART_Cmd(USART1,ENABLE);	//时能串口

}

/***********************************************************************
函数名称：void USART1_IRQHandler(void) 
功    能：完成SCI的数据的接收，并做标识
输入参数：
输出参数：
编写时间：2012.11.22
编 写 人：
注    意  RS232用的是USART1.
***********************************************************************/
//void USART1_IRQHandler(void)  
//{
//	unsigned char temp;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{	
//		temp=USART1->DR;
//		USART_SendData(USART1,temp);
//		RS232_buff[RS232_rec_counter] = temp;//
//		RS232_rec_counter ++;
///********以RS232_END_FLAG1和RS232_END_FLAG2定义的字符作为一帧数据的结束标识************/
//		if(RS232_rec_counter >= 2)	//只有接收到2个数据以上才做判断
//		{
//			if(RS232_buff[RS232_rec_counter - 2] == RS232_END_FLAG1 && RS232_buff[RS232_rec_counter - 1] == RS232_END_FLAG2) 	//帧起始标志   
//			{
//				RS232_REC_Flag = 1;
//			}
//		}
//		if(RS232_rec_counter > RS232_REC_BUFF_SIZE)//超过接收缓冲区大小
//		{
//			RS232_rec_counter = 0;
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
//	{
//        USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
//    }	
//}

/***********************************************************************
*	函数名称：void USART1_IRQHandler(void) 
*	功    能：完成SCI的数据的接收，并做标识
*	形	  参:无
*	返 回 值:无
***********************************************************************/
void USART1_IRQHandler(void) 
{

	unsigned char temp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		temp = USART1->DR;
		
		if(temp==RS232_START_FLAG1)
		{	
			recstatu = 1;
			RS232_rec_counter=0;
			packerflag=0;
			return;

		}
		if(temp==RS232_END_FLAG2)
		{
			Uart_Size=RS232_rec_counter;
			recstatu = 0;
			packerflag=1;
			RS232_rec_counter=0;
			/* 开机命令 */
			if(memcmp((void*)RS232_buff,RXD_1,Uart_Size)==0)
			{
					StaBy_Flag=1;
					Stop_Flag=0;
					Start_Flag=0;
			}
			/* 开灯命令 */
			if(memcmp((void*)RS232_buff,Start_RXD_1,Uart_Size)==0)
			{
					StaBy_Flag=0;
					Stop_Flag=0;	
					Start_Flag=1;
			}
			/* 关灯命令 */
			if(memcmp((void*)RS232_buff,Stop_RXD_1,Uart_Size)==0)
			{
					StaBy_Flag=0;
					Start_Flag=0;
					Stop_Flag=1;		
			}			
			return;
		}
		if(recstatu==1)
		{
			RS232_buff[RS232_rec_counter++] = temp;
		}

		}
}

/***********************************************************************
函数名称：RS232_Send_Data(unsigned char *send_buff,unsigned int length)
功    能：RS232发送字符串
输入参数：send_buff:待发送的数据指针；length：发送的数据长度（字符个数）
输出参数：
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
void RS232_Send_Data(unsigned char *send_buff,unsigned int length)
{
 	unsigned int i = 0;
	for(i = 0;i < length;i ++)
	{			
		USART1->DR = send_buff[i];
		while((USART1->SR&0X40)==0);	
	}	
}

void RS232_Send_Byte( char Send_Byte)
{
		USART1->DR = Send_Byte;
		while((USART1->SR&0X40)==0);	
}

/***********************************************************************
函数名称：void RS485_Delay(uint32_t nCount)
功    能：RS485收发延时函数
输入参数：
输出参数：
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
  void RS485_Delay(uint32_t nCount)
 { 
   while(nCount > 0)
   { 
   	  nCount --;   
   }
 }
