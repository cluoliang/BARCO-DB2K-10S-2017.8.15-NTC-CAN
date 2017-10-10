/***********************************************************************
文件名称：adc.h
功    能：led  IO初始化
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
#ifndef _ADC_H_
#define _ADC_H_
#define ADC1_DR_Address    ((u32)0x4001204C)
extern __IO uint16_t ADC_ConvertedValue[2];
extern __IO uint8_t sample_finish;
extern __IO uint8_t sample_index;
extern uint32_t sample_1;
extern uint32_t sample_2;
void ADC_Configuration(void);
void user_adc_init(void);
void adc_gpio_init(void);
void adc_config(void);
void adc_dma_init(void);
void adc_timer_init(uint16_t arr,uint16_t psc);
#endif

