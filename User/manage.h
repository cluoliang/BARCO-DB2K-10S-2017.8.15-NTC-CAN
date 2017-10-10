#ifndef _MANAGE_H
#define _MANAGE_H
extern volatile uint8_t Temp_Save_Flag;
#define StandBy_Mode 0x01
#define Run_Mode 0x02
#define Stop_Mode 0x03
uint8_t Search_Mode(uint8_t *Buff,uint8_t Size);
uint8_t Manage_Mode(uint8_t Mode);
uint8_t Manage_Temp(void);
void Manage_Light(void);
#endif

