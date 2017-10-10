
#include "main.H"
/***********************************************************************
�ļ����ƣ�SCI.C
��    �ܣ���ɶ�usart1��usart2�Ĳ���
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺��������ͨ���ж������ض����ַ���ȷ��һ֡�����Ƿ�����ġ�
USART1 ʱ�� : RCC_APB2PeriphClockCmd
USART1~6 ʱ�� :RCC_APB1PeriphClockCmd
***********************************************************************/

volatile unsigned char RS232_REC_Flag = 0;
volatile unsigned char RS485_REC_Flag = 0;
volatile  unsigned char RS232_buff[RS232_REC_BUFF_SIZE];//���ڽ�������
volatile unsigned char RS485_buff[RS485_REC_BUFF_SIZE];//���ڽ�������
volatile unsigned int RS232_rec_counter = 0;//����RS232���ռ���
volatile unsigned int RS485_rec_counter = 0;//����RS485���ռ���

volatile unsigned char RS232_buff_bite;//���ڽ����������ַ�
unsigned char recstatu;	//�����ж��Ƿ���һ�����ڽ������ݰ���״̬
volatile unsigned char packerflag;	//�Ƿ���ܵ����������ݰ�
unsigned char Uart_Size=0;

volatile unsigned char StaBy_Flag = 0; /*����״̬��*/
volatile unsigned char count = 0;
volatile unsigned char Start_Flag = 0;/*����״̬��*/
volatile unsigned char Stop_Flag = 0;/*�ص�״̬��*/

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
	
	GPIO_InitTypeDef GPIO_InitStructure;	//����GPIO_InitTypeDef���͵Ľṹ���ԱGPIO_InitStructure
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//ʹ����Ҫ�õ���GPIO�ܽ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1 ʱ��
	
	USART_DeInit(USART1);	//��λ����1	
	USART_StructInit(&USART_InitStructure);	//����Ĭ��USART����
	USART_ClockStructInit(&USART_ClockInitStruct);	//����Ĭ��USART���� 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //�ܽ�PA9����ΪUSART1
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 	//���ô���1�Ĺܽ� PA8 USART1_EN PA9 USART1_TX PA10 USART1_RX  
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
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //�����ж�ʹ��
	USART_ClearITPendingBit(USART1, USART_IT_TC);	//����ж�TCλ
	USART_Cmd(USART1,ENABLE);	//ʱ�ܴ���

}

/***********************************************************************
�������ƣ�void USART1_IRQHandler(void) 
��    �ܣ����SCI�����ݵĽ��գ�������ʶ
���������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    ��  RS232�õ���USART1.
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
///********��RS232_END_FLAG1��RS232_END_FLAG2������ַ���Ϊһ֡���ݵĽ�����ʶ************/
//		if(RS232_rec_counter >= 2)	//ֻ�н��յ�2���������ϲ����ж�
//		{
//			if(RS232_buff[RS232_rec_counter - 2] == RS232_END_FLAG1 && RS232_buff[RS232_rec_counter - 1] == RS232_END_FLAG2) 	//֡��ʼ��־   
//			{
//				RS232_REC_Flag = 1;
//			}
//		}
//		if(RS232_rec_counter > RS232_REC_BUFF_SIZE)//�������ջ�������С
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
*	�������ƣ�void USART1_IRQHandler(void) 
*	��    �ܣ����SCI�����ݵĽ��գ�������ʶ
*	��	  ��:��
*	�� �� ֵ:��
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
			/* �������� */
			if(memcmp((void*)RS232_buff,RXD_1,Uart_Size)==0)
			{
					StaBy_Flag=1;
					Stop_Flag=0;
					Start_Flag=0;
			}
			/* �������� */
			if(memcmp((void*)RS232_buff,Start_RXD_1,Uart_Size)==0)
			{
					StaBy_Flag=0;
					Stop_Flag=0;	
					Start_Flag=1;
			}
			/* �ص����� */
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
�������ƣ�RS232_Send_Data(unsigned char *send_buff,unsigned int length)
��    �ܣ�RS232�����ַ���
���������send_buff:�����͵�����ָ�룻length�����͵����ݳ��ȣ��ַ�������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
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
�������ƣ�void RS485_Delay(uint32_t nCount)
��    �ܣ�RS485�շ���ʱ����
���������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
***********************************************************************/
  void RS485_Delay(uint32_t nCount)
 { 
   while(nCount > 0)
   { 
   	  nCount --;   
   }
 }
