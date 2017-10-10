#include "main.h"



/***********************************************************************
函数名称：TIM_Configuration(unsigned int time)
功    能：定时器2配置
输入参数：time：定时时间，单位ms。
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器2!
***********************************************************************/
void TIM_Configuration(u16 arr,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	TIM_Cmd(TIM2,DISABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
}

/***********************************************************************
函数名称：TIM2_IRQHandler(void)
功    能：定时器2中断函数
输入参数：每隔	time（ms）中断一次
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
//void TIM2_IRQHandler(void)
//{

//// 	CLI();			//关闭总中断
//if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) 
//{
//	TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);

//	GPIO_ToggleBits(LED1);
//	GPIO_ToggleBits(LED2);
//	GPIO_ToggleBits(LED3);
//	GPIO_ToggleBits(LED4);
//}
//// 	SEI();			//打开总中断	
//}
