


#include "main.h"

int main(void)
{	

	uint8_t Mode=0;
	LED_Configuration();
	Relay_Gpio_Init();
	TIM_Config();
	Pwm_Init(50,500,500);
	USART_Configuration(115200);
	CAN_Config();
	user_adc_init();
	NVIC_Config();
	#ifdef Use_Tsl2581
	Tsl2581_Basic_Init();
	ReadBuffer=I2C_DEV_Read(COMMAND_CMD | TRANSACTION |ID);
	I2C_DEV_init();
	#endif

	while(1)
	{	
		
		Mode=Search_Mode((void*)RS232_buff,Uart_Size);
		Manage_Mode(Mode);
		/* ÎÂ¶È²É¼¯ */
		Manage_Temp();
	}
}
