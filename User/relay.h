#ifndef _RELAY_H
#define _RELAY_H


#define Relay1		GPIOC , GPIO_Pin_6
#define Relay2		GPIOC , GPIO_Pin_7


#define Relay1_Off 		GPIO_ResetBits(GPIOC , GPIO_Pin_6)
#define Relay2_Off 		GPIO_ResetBits(GPIOC , GPIO_Pin_7)


#define Relay1_On 		GPIO_SetBits(GPIOC , GPIO_Pin_6)
#define Relay2_On 		GPIO_SetBits(GPIOC , GPIO_Pin_7)


void Relay_Gpio_Init(void);

#endif


