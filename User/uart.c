
#include "main.h"



volatile unsigned char RS232_buff[RS232_REC_BUFF_SIZE];//���ڽ�������
volatile unsigned int RS232_rec_counter = 0;//����RS232���ռ���
volatile unsigned char RS232_buff_bite;//���ڽ����������ַ�

volatile unsigned char recstatu;	//�����ж��Ƿ���һ�����ڽ������ݰ���״̬
volatile unsigned char packerflag;	//�Ƿ���ܵ����������ݰ�
volatile unsigned char Uart_Size;

volatile unsigned char StaBy_Flag = 0; /*����״̬��*/
volatile unsigned char count = 0;
volatile unsigned char Start_Flag = 0;/*����״̬��*/
volatile unsigned char Stop_Flag = 0;/*�ص�״̬��*/

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

void USART_Configuration(int baudrate)
{ 
	
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO_InitTypeDef���͵Ľṹ���ԱGPIO_InitStructure

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	//ʹ����Ҫ�õ���GPIO�ܽ�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);
	//ʹ��USART1 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	///��λ����1
	USART_DeInit(USART1);
	
	USART_StructInit(&USART_InitStructure);//����Ĭ��USART����
	USART_ClockStructInit(&USART_ClockInitStruct);//����Ĭ��USART����        
	//���ô���1�Ĺܽ� PA8 USART1_EN PA9 USART1_TX PA10 USART1_RX    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);        //�ܽ�PA9����ΪUSART1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                                                                                                 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	USART_ClockInit(USART1,&USART_ClockInitStruct);


	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//USART_InitStructure.USART_Parity = USART_Parity_Even;
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure); 

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        ///////�����ж�ʹ��
	USART_ClearITPendingBit(USART1, USART_IT_TC);//����ж�TCλ
	USART_Cmd(USART1,ENABLE);//���ʹ�ܴ�
	//NVIC_Configuration();
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
			return;
		}
		if(recstatu==1)
		{
			RS232_buff[RS232_rec_counter++] = temp;
		}

		}	
}


unsigned char RS232_Accept_byte(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		RS232_buff_bite = USART1->DR;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
	return RS232_buff_bite;
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

