#include "main.h"



/***********************************************************************
�������ƣ�TIM_Configuration(unsigned int time)
��    �ܣ���ʱ��2����
���������time����ʱʱ�䣬��λms��
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��2!
***********************************************************************/
void TIM_Configuration(u16 arr,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
	TIM_Cmd(TIM2,DISABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
}

/***********************************************************************
�������ƣ�TIM2_IRQHandler(void)
��    �ܣ���ʱ��2�жϺ���
���������ÿ��	time��ms���ж�һ��
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
//void TIM2_IRQHandler(void)
//{

//// 	CLI();			//�ر����ж�
//if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) 
//{
//	TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);

//	GPIO_ToggleBits(LED1);
//	GPIO_ToggleBits(LED2);
//	GPIO_ToggleBits(LED3);
//	GPIO_ToggleBits(LED4);
//}
//// 	SEI();			//�����ж�	
//}
