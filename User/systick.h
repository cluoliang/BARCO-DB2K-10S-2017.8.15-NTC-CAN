#ifndef _SYSTICK_H
#define _SYSTICK_H
#include "main.h"

#define Delay_ms(x) Delay_us(1000*x)	 //µ¥Î»ms

#define ms_1 1000
#define ms_10 100
#define ms_100 10
#define us_10 1000000
void systick(uint32_t tick);
void Delay_us(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void Stop_systick(void);
void Stop(uint16_t time);
#endif
