/***********************************************************************
�ļ����ƣ�DS18B20.C
��    �ܣ���ɶԶ�DS18B20�Ļ�������
��дʱ�䣺2017.3.15
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "main.h"	

unsigned char Temp_Flag_1=0;
uint8_t g_rom_id_1[8] = {0};
uint8_t Temp_Hex_1 = 0;
uint8_t ID_Buff_1[MAXNUM][8];
//uint8_t dat_1[7]={0x28,0xff,0xf4,0x7f,0xb3,0x16,0x03};
/*********************************************************************************************************
*	�� �� ��: bsp_InitDS18B20
*	����˵��: ����STM32��GPIO��SPI�ӿڣ��������� DS18B20
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/


/*********************************************************************************************************
*	�� �� ��: bsp_InitDS18B20
*	����˵��: ����STM32��GPIO��SPI�ӿڣ��������� DS18B20
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DS18B20_GPIO_Configuration_1(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(DS18B20_CLK_1, ENABLE); 													   
 	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN_1);	 
}

static void DS18B20_Mode_IPU_1(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Mode_Out_PP_1
*	����˵��: �������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void DS18B20_Mode_Out_PP_1(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;									   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Reset_1
*	����˵��: ��λDS18B20�� ����DQΪ�ͣ���������480us��Ȼ��ȴ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void  DS18B20_Reset_1(void)
{
	DS18B20_Mode_Out_PP_1();	//��������Ϊ�������
	DS18B20_DATA_OUT_1(LOW);	//�������ٲ���480us�ĵ͵�ƽ��λ�ź�
	Delay_us(750);	//�ӳ�750us
	DS18B20_DATA_OUT_1(HIGH);	//����������λ�ź�֮���轫�������ߣ�������ʱ15us
	Delay_us(15);	//�ӳ�15us�󣬵ȴ����߱�DS18B20����
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Presence_1
*	����˵��: ��������Ƿ�DS18B20����
*	��    ��: ��
*	�� �� ֵ: 0:�ɹ���1:ʧ��
*********************************************************************************************************
*/
static uint8_t DS18B20_Presence_1(void)
{
	uint8_t pulse_time = 0;
		
	
	DS18B20_Mode_IPU_1();	//��������Ϊ��������
	/*�ȴ�������ĵ���������һ��60~240us�ĵ͵�ƽ�źţ������������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us�󷢸�����һ���͵�ƽ����*/	
	while( DS18B20_DATA_IN_1() && pulse_time<100 )	
	{
		pulse_time++;	//
		Delay_us(1);
	}	
	if( pulse_time >=100 )

		return 1;
	else
		pulse_time = 0;

	while( !DS18B20_DATA_IN_1() && pulse_time<240 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
	}



/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Read_Bit_1
*	����˵��: ��ȡDS18B20����һλ����
*	��    ��: ��
*	�� �� ֵ: dat_1
*********************************************************************************************************
*/

static uint8_t DS18B20_Read_Bit_1(void)
{
	uint8_t dat_1;
	DS18B20_Mode_Out_PP_1();	//��0��1��ʱ������Ҫ����60us
	DS18B20_DATA_OUT_1(LOW);
	Delay_us(10);	//��������һ��1~15us�ĵ͵�ƽ�ź�
	
	DS18B20_Mode_IPU_1();	//���ó�������ͷ����ߣ����ⲿ�������轫��������
	
	if( DS18B20_DATA_IN_1() == SET )
		dat_1 = 1;
	else
		dat_1 = 0;
	Delay_us(45);
	
	return dat_1;
}



/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Read_Byte_1
*	����˵��: ��DS18B20��ȡ1�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 uint8_t DS18B20_Read_Byte_1(void)
{
	uint8_t i, j, dat_1 = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit_1();		
		dat_1 = (dat_1) | (j<<i);
	}
	
	return dat_1;
}


/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Read2Bit_1
*	����˵��: ��DS18B20��ȡ2��λ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/  

uint8_t DS18B20_Read2Bit_1(void)  
{  
    uint8_t i,dat_1a = 0;   
    for(i = 0;i < 2;i ++)  
    {  
        dat_1a <<= 1;  
        if(DS18B20_Read_Bit_1())
				{
					dat_1a = dat_1a|1;
				}					  
    }  
    return dat_1a;  
}


/*
*********************************************************************************************************
*	�� �� ��: DS18B20_WriteByte
*	����˵��: ��DS18B20д��1�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void DS18B20_Write_Byte_1(uint8_t dat_1)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP_1();
	for( i=0; i<8; i++ )
	{
		testb = dat_1&0x01;
		dat_1 = dat_1>>1;		
	/*д0��1��ʱ������Ҫ����60us*/
		if (testb)
		{			
			DS18B20_DATA_OUT_1(LOW);
			Delay_us(8);	//�����ʱ1~15us
			DS18B20_DATA_OUT_1(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT_1(LOW);
			
			Delay_us(70);	//�����ʱ 60us <  0 < 120us 
				
			DS18B20_DATA_OUT_1(HIGH); 		
			
			Delay_us(2);	//�ظ�ʱ�� 1us<Trec<�����
		}
	}

}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_WriteByte
*	����˵��: ��DS18B20д��1�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DS18B20_Write_Bit_1(uint8_t dat_1)
{

	DS18B20_Mode_Out_PP_1();
	
		if (dat_1)
		{			
			DS18B20_DATA_OUT_1(LOW);
			Delay_us(8);	//�����ʱ1~15us
			DS18B20_DATA_OUT_1(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT_1(LOW);
			
			Delay_us(70);	//�����ʱ 60us <  0 < 120us 
				
			DS18B20_DATA_OUT_1(HIGH); 		
			
			Delay_us(2);	//�ظ�ʱ�� 1us<Trec<�����
		}

}

void DS18B20_Start_1(void)
{
	DS18B20_Reset_1();	   
	DS18B20_Presence_1();	 
	DS18B20_Write_Byte_1(0XCC);		/* ����ROM */
	DS18B20_Write_Byte_1(0X44);		/* ��ʼת��*/
}

uint8_t DS18B20_Init_1(void)
{
	DS18B20_GPIO_Configuration_1();
	DS18B20_Reset_1();
	
	return DS18B20_Presence_1();
}




/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadTempReg_1
*	����˵��: ���¶ȼĴ�����ֵ��ԭʼ���ݣ�
*	��    ��: ��
*	�� �� ֵ: �¶ȼĴ������� ������16�õ� 1���϶ȵ�λ, Ҳ����С����ǰ�������)
*********************************************************************************************************
*/
float DS18B20_ReadTempReg_1(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Reset_1();	//��λDS18B20   
	DS18B20_Presence_1();	//�ȴ��͵�ƽʱ��
	DS18B20_Write_Byte_1(0XCC);				/* ����ROM */
	DS18B20_Write_Byte_1(0X44);				/* ��ʼת��*/
	
	DS18B20_Reset_1();
  DS18B20_Presence_1();
	DS18B20_Write_Byte_1(0XCC);				/* ����ROM */
  DS18B20_Write_Byte_1(0XBE);				/*��ȡ�¶�ֵ*/
	
	tplsb = DS18B20_Read_Byte_1();		 
	tpmsb = DS18B20_Read_Byte_1(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/*���¶�*/
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadID_1
*	����˵��: ��DS18B20��ROM ID�� �����ϱ���ֻ��1��оƬ
*	��    ��: _id �洢ID
*	�� �� ֵ: 0 ��ʾʧ�ܣ� 1��ʾ��⵽��ȷID
*********************************************************************************************************
*/
uint8_t DS18B20_ReadID_1(uint8_t *_id)
{
	uint8_t i;

	/* ���߸�λ */
	DS18B20_Reset_1();
	if (DS18B20_Presence_1() == 1)
	{
		return 0;
	}

	DS18B20_Write_Byte_1(0x33);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		_id[i] = DS18B20_Read_Byte_1();
	}

	DS18B20_Reset_1();		/* ���߸�λ */
	DS18B20_Presence_1();
	
	return 1;
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadTempByID_1
*	����˵��: ��ָ��ID���¶ȼĴ�����ֵ��ԭʼ���ݣ�
*	��    ��: ��
*	�� �� ֵ: �¶ȼĴ������� ������16�õ� 1���϶ȵ�λ, Ҳ����С����ǰ�������)
*********************************************************************************************************
*/
float DS18B20_ReadTempByID_1(uint8_t *_id)
{
	uint8_t temp1, temp2;
 
	short s_tem;
	float f_tem;

	uint8_t i;
	
	DS18B20_Reset_1();		/* ���߸�λ */
	DS18B20_Presence_1();
	DS18B20_Write_Byte_1(0x55);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte_1(_id[i]);
	}
	DS18B20_Write_Byte_1(0x44);	/* ��ת������ */
	DS18B20_Mode_Out_PP_1();
	DS18B20_DATA_OUT_1(HIGH); 
	Delay_ms(800);
	DS18B20_Reset_1();		/* ���߸�λ */
	DS18B20_Presence_1();
	DS18B20_Write_Byte_1(0x55);	/* ������ */
	
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte_1(_id[i]);
	}	
	DS18B20_Write_Byte_1(0xbe);

	temp1 = DS18B20_Read_Byte_1();	/* ���¶�ֵ���ֽ� */
	temp2 = DS18B20_Read_Byte_1();	/* ���¶�ֵ���ֽ� */
	
	DS18B20_Reset_1();
	DS18B20_Presence_1();

	s_tem = temp2<<8;
	s_tem = s_tem | temp1;
		if( s_tem < 0 ) 	/*���¶�*/
		{
			Temp_Flag_1=1;
			f_tem = (~s_tem+1) * 0.0625;
			Temp_Hex_1 = ~((uint8_t)f_tem)+1;
			
		}
		else
		{
			f_tem = s_tem * 0.0625;
			Temp_Hex_1 = f_tem;
		}
		CAN_COMMAND.Ambient_Temp = (uint8_t)Temp_Hex_1;
		return f_tem;	

	
}

/*
**********************************************************************
*	�������ƣ�unsigned char Read_ROM_ID_1(unsigned char * dat_1a)
*	��    �ܣ���1820��ROM id ����1����ɹ���id��dat_1a��ָλ�ã�����0ʧ�ܣ���dat_1aָ����ַ��0
*	��    ��: *dat_1a
*	�� �� ֵ: ��ȡ�ɹ�����1�����򷵻�0
***********************************************************************
*/
unsigned char Read_ROM_ID_1(unsigned char * dat_1a)
{
	/* �� ROM ID */
	uint8_t id[8];
	uint8_t ret;
	ret = DS18B20_ReadID_1(id);
	if (ret == 0)
	{
		memset(dat_1a, 0, 8);
		return 0;
	}
	else
	{
		memcpy(dat_1a, id, 8);
		return 1;
	}
}


/*
**********************************************************************
*	�������ƣ�unsigned char Read_ROM_ID_1(unsigned char * dat_1a)
*	��    �ܣ���1820��ROM id ����1����ɹ���id��dat_1a��ָλ�ã�����0ʧ�ܣ���dat_1aָ����ַ��0
*	��    ��: *dat_1a
*	�� �� ֵ: ��ȡ�ɹ�����1�����򷵻�0
***********************************************************************
*/  

uint8_t DS18B20_SearchROM_1(u8 (*pID)[8],u8 Num)  
{   
    unsigned char k,l=0,ConflictBit,m,n;  
    unsigned char BUFFER[MAXNUM];  
    unsigned char ss[64];
//		unsigned char s=0;  
    uint8_t num = 0;      
    do  
    {  
        DS18B20_Reset_1();								//��λDS18B20����
				Delay_us(480);
        DS18B20_Write_Byte_1(0xF0);	//����ROM  
        for(m=0;m<8;m++)  
        {  
            unsigned char s=0;  
            for(n=0;n<8;n++)  
            {  
                k=DS18B20_Read2Bit_1();		// ��ȡ��λ����
                k=k&0x03;  
                s= s>>1;  
                if(k==0x01)							//0000 0001 �������������Ϊ0
                {             
                    DS18B20_Write_Bit_1(0);//д0��ʹ������Ϊ0��������Ӧ
                    ss[(m*8+n)]=0;  
                }  
                else if(k==0x02)				//0000 0010 �������������Ϊ1
                {  
                    s=s|0x80;  
                    DS18B20_Write_Bit_1(1);//д1��ʹ������Ϊ1��������Ӧ
                    ss[(m*8+n)]=1;  
                }  
                else if(k==0x00)//�����ȡ��������Ϊ00�����г�ͻ����Ҫ���г�ͻλ�ж�
                {                
                    ConflictBit=m*8+n+1;                   
                    if(ConflictBit>BUFFER[l])//�����ͻλ����ջ������д0
                    {                         
                        DS18B20_Write_Bit_1(0);  
                        ss[(m*8+n)]=0;                                                
                        BUFFER[++l]=ConflictBit;                         
                    }  
                    else if(ConflictBit<BUFFER[l])//�����ͻλС��ջ������д��ǰ������
                    {  
                        s=s|((ss[(m*8+n)]&0x01)<<7);  
                        DS18B20_Write_Bit_1(ss[(m*8+n)]);  
                    }  
                    else if(ConflictBit==BUFFER[l])//�����ͻλ����ջ������д1
                    {  
                        s=s|0x80;  
                        DS18B20_Write_Bit_1(1);  
                        ss[(m*8+n)]=1;  
                        l=l-1;  
                    }  
                }  
                else//�������������Ϊ0x03����˵���������ϲ������κ��豸
                {  
                    return num; //������ɣ������������ĸ���
                }
								Delay_us(5);
            }  
            pID[num][m]=s;
						s=0;
        }  
        num=num+1;
    }  
    while(BUFFER[l]!=0&&(num<MAXNUM));   
      
    return num;     //�����������ĸ���
}


/*
**********************************************************************
*	��������:�DS18B20_ReadDesignateTemper_1
*	��    �ܣ���1820��ROM id ����1����ɹ���id��dat_1a��ָλ�ã�����0ʧ�ܣ���dat_1aָ����ַ��0
*	��    ��: *dat_1a
*	�� �� ֵ: ��ȡ�ɹ�����1�����򷵻�0
***********************************************************************
*/   

//float DS18B20_ReadDesignateTemper_1(uint8_t pID[8])  
//{  
//    uint8_t th, tl;  
//    uint16_t dat_1a;  
//		float f_tem;
//		DS18B20_Reset_1();
//      
//    if(DS18B20_Presence_1() == 1)				//�����ж�   
//    {  
//        return 0xffff;              		//���ش�����Ϣ
//    }  
//  
////		DS18B20_Write_Byte_1(0XCC);				/* ����ROM */
////		DS18B20_Write_Byte_1(0x44);				/*�����¶�ת��*/
////    DS18B20_Reset_1();  
//    DS18B20_Write_Byte_1(0x55);       //ƥ��ROMָ��
//    for(dat_1a = 0;dat_1a < 8;dat_1a ++)   		//����8�ֽڵ����к�
//    {  
//       DS18B20_Write_Byte_1(pID[dat_1a]);  
//    }  
//    Delay_us(10);  
//    DS18B20_Write_Byte_1(0xBE);    //��ȡ�¶�ָ�� 
//    tl = DS18B20_Read_Byte_1();    				//��8λ
//    th = DS18B20_Read_Byte_1();    				//��8λ
//    dat_1a = th; 
//		dat_1a = dat_1a<<8;
//    dat_1a = dat_1a|tl;  
//		if( dat_1a < 0 ) 	/*���¶�*/
//		{
//			Temp_Flag_1=1;
//			f_tem = (~dat_1a+1) * 0.0625;
//		}
//		else
//			f_tem = dat_1a * 0.0625;
//		return f_tem;	
//}		

float DS18B20_ReadDesignateTemper_1(uint8_t pID[8])  
{  
	uint8_t temp1, temp2;
 
	short s_tem;
	float f_tem;

	uint8_t i;
	
	DS18B20_Reset_1();		/* ���߸�λ */
	DS18B20_Presence_1();
	DS18B20_Write_Byte_1(0x55);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte_1(pID[i]);
	}
	DS18B20_Write_Byte_1(0x44);	/* ��ת������ */
	DS18B20_Mode_Out_PP_1();
	DS18B20_DATA_OUT_1(HIGH); 
	Delay_ms(800);
	DS18B20_Reset_1();		/* ���߸�λ */
	DS18B20_Presence_1();
	DS18B20_Write_Byte_1(0x55);	/* ������ */
	
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte_1(pID[i]);
	}	
	DS18B20_Write_Byte_1(0xbe);

	temp1 = DS18B20_Read_Byte_1();	/* ���¶�ֵ���ֽ� */
	temp2 = DS18B20_Read_Byte_1();	/* ���¶�ֵ���ֽ� */
	
	DS18B20_Reset_1();
	DS18B20_Presence_1();

	s_tem = temp2<<8;
	s_tem = s_tem | temp1;
		if( s_tem < 0 ) 	/*���¶�*/
		{
			Temp_Flag_1=1;
			f_tem = (~s_tem+1) * 0.0625;
		}
		else
			f_tem = s_tem * 0.0625;
		return f_tem;	
}		

//uint8_t CheckCrc_1(void)
//{
//	uint8_t crc = 0,i,j;
//	for(i=0;i<sizeof(dat_1)/sizeof(dat_1[0]);i++)
//	{
//		crc=crc^dat_1[i];
//		for(j=0;j<8;j++)
//		{
//			if(crc&0x01)crc=(crc>>1)^0x8c;
//				else crc>>=1;
//		}
//	
//		}
//	return crc;

//}
