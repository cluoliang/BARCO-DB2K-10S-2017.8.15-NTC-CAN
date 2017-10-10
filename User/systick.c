#include "main.h"

static __IO uint32_t TimingDelay;

/***********************************************************************
Fun name:systick(void)

Abstract:Configure systick at 1ms

Parameters:systick

Returns:None
***********************************************************************/
void systick(uint32_t tick)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / tick);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
//	printf("clk=%d\r\n",RCC_Clocks.SYSCLK_Frequency);
}



/***********************************************************************
Fun name:Delay(void)

Abstract:Delay at nTime

Parameters:nTime

Returns:None
***********************************************************************/
void Delay_us(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
}

/***********************************************************************
Fun name:TimingDelay_Decrement(void)

Abstract:The fun is called in systick  interrupt

Parameters:None

Returns:None
***********************************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
 }
 
 
void Stop_systick(void)
{
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL &= ~ SysTick_CTRL_TICKINT_Msk;
}

void Stop(uint16_t time)
{
	uint16_t i = 0;
	while(time--)
	{
		i=12000;
		while(i--);
	}
	
}
