#ifndef __EXIT_H
#define __EXIT_H

void EXTI_Configuration(void);
void SystemInitFormStop(void);
void GPIO_Set(void);
void StopMode_Measure(void);
void EXTI0_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
#endif
