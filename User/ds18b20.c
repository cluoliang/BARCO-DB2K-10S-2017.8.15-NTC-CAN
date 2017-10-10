/***********************************************************************
�ļ����ƣ�DS18B20.C
��    �ܣ���ɶԶ�DS18B20�Ļ�������
��дʱ�䣺2017.3.15
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "main.h"	

unsigned char Temp_Flag=0;
uint8_t g_rom_id[8] = {0};
uint8_t Temp_Hex = 0;
uint8_t ID_Buff[MAXNUM][8];
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
void DS18B20_GPIO_Configuration(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(DS18B20_CLK, ENABLE); 													   
 	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 
}

static void DS18B20_Mode_IPU(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Mode_Out_PP
*	����˵��: �������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void DS18B20_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;									   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Reset
*	����˵��: ��λDS18B20�� ����DQΪ�ͣ���������480us��Ȼ��ȴ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void  DS18B20_Reset(void)
{
	DS18B20_Mode_Out_PP();	//��������Ϊ�������
	DS18B20_DATA_OUT(LOW);	//�������ٲ���480us�ĵ͵�ƽ��λ�ź�
	Delay_us(750);	//�ӳ�750us
	DS18B20_DATA_OUT(HIGH);	//����������λ�ź�֮���轫�������ߣ�������ʱ15us
	Delay_us(15);	//�ӳ�15us�󣬵ȴ����߱�DS18B20����
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Presence
*	����˵��: ��������Ƿ�DS18B20����
*	��    ��: ��
*	�� �� ֵ: 0:�ɹ���1:ʧ��
*********************************************************************************************************
*/
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
		
	
	DS18B20_Mode_IPU();	//��������Ϊ��������
	/*�ȴ�������ĵ���������һ��60~240us�ĵ͵�ƽ�źţ������������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us�󷢸�����һ���͵�ƽ����*/	
	while( DS18B20_DATA_IN() && pulse_time<100 )	
	{
		pulse_time++;	//
		Delay_us(1);
	}	
	if( pulse_time >=100 )

		return 1;
	else
		pulse_time = 0;

	while( !DS18B20_DATA_IN() && pulse_time<240 )
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
*	�� �� ��: DS18B20_Read_Bit
*	����˵��: ��ȡDS18B20����һλ����
*	��    ��: ��
*	�� �� ֵ: dat
*********************************************************************************************************
*/

static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	DS18B20_Mode_Out_PP();	//��0��1��ʱ������Ҫ����60us
	DS18B20_DATA_OUT(LOW);
	Delay_us(10);	//��������һ��1~15us�ĵ͵�ƽ�ź�
	
	DS18B20_Mode_IPU();	//���ó�������ͷ����ߣ����ⲿ�������轫��������
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	Delay_us(45);
	
	return dat;
}



/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Read_Byte
*	����˵��: ��DS18B20��ȡ1�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}


/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Read2Bit
*	����˵��: ��DS18B20��ȡ2��λ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/  

uint8_t DS18B20_Read2Bit(void)  
{  
    uint8_t i,data = 0;   
    for(i = 0;i < 2;i ++)  
    {  
        data <<= 1;  
        if(DS18B20_Read_Bit())
				{
					data = data|1;
				}					  
    }  
    return data;  
}


/*
*********************************************************************************************************
*	�� �� ��: DS18B20_WriteByte
*	����˵��: ��DS18B20д��1�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
	/*д0��1��ʱ������Ҫ����60us*/
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			Delay_us(8);	//�����ʱ1~15us
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);
			
			Delay_us(70);	//�����ʱ 60us <  0 < 120us 
				
			DS18B20_DATA_OUT(HIGH); 		
			
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
void DS18B20_Write_Bit(uint8_t dat)
{

	DS18B20_Mode_Out_PP();
	
		if (dat)
		{			
			DS18B20_DATA_OUT(LOW);
			Delay_us(8);	//�����ʱ1~15us
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);
			
			Delay_us(70);	//�����ʱ 60us <  0 < 120us 
				
			DS18B20_DATA_OUT(HIGH); 		
			
			Delay_us(2);	//�ظ�ʱ�� 1us<Trec<�����
		}

}

void DS18B20_Start(void)
{
	DS18B20_Reset();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* ����ROM */
	DS18B20_Write_Byte(0X44);		/* ��ʼת��*/
}

uint8_t DS18B20_Init(void)
{
	DS18B20_GPIO_Configuration();
	DS18B20_Reset();
	
	return DS18B20_Presence();
}




/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadTempReg
*	����˵��: ���¶ȼĴ�����ֵ��ԭʼ���ݣ�
*	��    ��: ��
*	�� �� ֵ: �¶ȼĴ������� ������16�õ� 1���϶ȵ�λ, Ҳ����С����ǰ�������)
*********************************************************************************************************
*/
float DS18B20_ReadTempReg(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Reset();	//��λDS18B20   
	DS18B20_Presence();	//�ȴ��͵�ƽʱ��
	DS18B20_Write_Byte(0XCC);				/* ����ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת��*/
	
	DS18B20_Reset();
  DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ����ROM */
  DS18B20_Write_Byte(0XBE);				/*��ȡ�¶�ֵ*/
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
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
*	�� �� ��: DS18B20_ReadID
*	����˵��: ��DS18B20��ROM ID�� �����ϱ���ֻ��1��оƬ
*	��    ��: _id �洢ID
*	�� �� ֵ: 0 ��ʾʧ�ܣ� 1��ʾ��⵽��ȷID
*********************************************************************************************************
*/
uint8_t DS18B20_ReadID(uint8_t *_id)
{
	uint8_t i;

	/* ���߸�λ */
	DS18B20_Reset();
	if (DS18B20_Presence() == 1)
	{
		return 0;
	}

	DS18B20_Write_Byte(0x33);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		_id[i] = DS18B20_Read_Byte();
	}

	DS18B20_Reset();		/* ���߸�λ */
	DS18B20_Presence();
	
	return 1;
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadTempByID
*	����˵��: ��ָ��ID���¶ȼĴ�����ֵ��ԭʼ���ݣ�
*	��    ��: ��
*	�� �� ֵ: �¶ȼĴ������� ������16�õ� 1���϶ȵ�λ, Ҳ����С����ǰ�������)
*********************************************************************************************************
*/
float DS18B20_ReadTempByID(uint8_t *_id)
{
	uint8_t temp1, temp2;
 
	short s_tem;
	float f_tem;

	uint8_t i;
	
	DS18B20_Reset();		/* ���߸�λ */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(_id[i]);
	}
	DS18B20_Write_Byte(0x44);	/* ��ת������ */
	DS18B20_Mode_Out_PP();
	DS18B20_DATA_OUT(HIGH); 
	Delay_ms(800);
	DS18B20_Reset();		/* ���߸�λ */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* ������ */
	
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(_id[i]);
	}	
	DS18B20_Write_Byte(0xbe);

	temp1 = DS18B20_Read_Byte();	/* ���¶�ֵ���ֽ� */
	temp2 = DS18B20_Read_Byte();	/* ���¶�ֵ���ֽ� */
	
	DS18B20_Reset();
	DS18B20_Presence();

	s_tem = temp2<<8;
	s_tem = s_tem | temp1;
		if( s_tem < 0 ) 	/*���¶�*/
		{
			Temp_Flag=1;
			f_tem = (~s_tem+1) * 0.0625;

			Temp_Hex = ~((uint8_t)f_tem)+1;

		}
		else
		{	
			f_tem = s_tem * 0.0625;
			Temp_Hex = f_tem;
		}
		CAN_COMMAND.Optical_Temp = (uint8_t)Temp_Hex;
		return f_tem;	

	
}

/*
**********************************************************************
*	�������ƣ�unsigned char Read_ROM_ID(unsigned char * data)
*	��    �ܣ���1820��ROM id ����1����ɹ���id��data��ָλ�ã�����0ʧ�ܣ���dataָ����ַ��0
*	��    ��: *data
*	�� �� ֵ: ��ȡ�ɹ�����1�����򷵻�0
***********************************************************************
*/
unsigned char Read_ROM_ID(unsigned char * data)
{
	/* �� ROM ID */
	uint8_t id[8];
	uint8_t ret;
	ret = DS18B20_ReadID(id);
	if (ret == 0)
	{
		memset(data, 0, 8);
		return 0;
	}
	else
	{
		memcpy(data, id, 8);
		return 1;
	}
}


/*
**********************************************************************
*	�������ƣ�unsigned char Read_ROM_ID(unsigned char * data)
*	��    �ܣ���1820��ROM id ����1����ɹ���id��data��ָλ�ã�����0ʧ�ܣ���dataָ����ַ��0
*	��    ��: *data
*	�� �� ֵ: ��ȡ�ɹ�����1�����򷵻�0
***********************************************************************
*/  

uint8_t DS18B20_SearchROM(u8 (*pID)[8],u8 Num)  
{   
    unsigned char k,l=0,ConflictBit,m,n;  
    unsigned char BUFFER[MAXNUM];  
    unsigned char ss[64];
//		unsigned char s=0;  
    uint8_t num = 0;      
    do  
    {  
        DS18B20_Reset();								//��λDS18B20����
				Delay_us(480);
        DS18B20_Write_Byte(0xF0);	//����ROM  
        for(m=0;m<8;m++)  
        {  
            unsigned char s=0;  
            for(n=0;n<8;n++)  
            {  
                k=DS18B20_Read2Bit();		// ��ȡ��λ����
                k=k&0x03;  
                s= s>>1;  
                if(k==0x01)							//0000 0001 �������������Ϊ0
                {             
                    DS18B20_Write_Bit(0);//д0��ʹ������Ϊ0��������Ӧ
                    ss[(m*8+n)]=0;  
                }  
                else if(k==0x02)				//0000 0010 �������������Ϊ1
                {  
                    s=s|0x80;  
                    DS18B20_Write_Bit(1);//д1��ʹ������Ϊ1��������Ӧ
                    ss[(m*8+n)]=1;  
                }  
                else if(k==0x00)//�����ȡ��������Ϊ00�����г�ͻ����Ҫ���г�ͻλ�ж�
                {                
                    ConflictBit=m*8+n+1;                   
                    if(ConflictBit>BUFFER[l])//�����ͻλ����ջ������д0
                    {                         
                        DS18B20_Write_Bit(0);  
                        ss[(m*8+n)]=0;                                                
                        BUFFER[++l]=ConflictBit;                         
                    }  
                    else if(ConflictBit<BUFFER[l])//�����ͻλС��ջ������д��ǰ������
                    {  
                        s=s|((ss[(m*8+n)]&0x01)<<7);  
                        DS18B20_Write_Bit(ss[(m*8+n)]);  
                    }  
                    else if(ConflictBit==BUFFER[l])//�����ͻλ����ջ������д1
                    {  
                        s=s|0x80;  
                        DS18B20_Write_Bit(1);  
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
*	��������:�DS18B20_ReadDesignateTemper
*	��    �ܣ���1820��ROM id ����1����ɹ���id��data��ָλ�ã�����0ʧ�ܣ���dataָ����ַ��0
*	��    ��: *data
*	�� �� ֵ: ��ȡ�ɹ�����1�����򷵻�0
***********************************************************************
*/   

//float DS18B20_ReadDesignateTemper(uint8_t pID[8])  
//{  
//    uint8_t th, tl;  
//    uint16_t data;  
//		float f_tem;
//		DS18B20_Reset();
//      
//    if(DS18B20_Presence() == 1)				//�����ж�   
//    {  
//        return 0xffff;              		//���ش�����Ϣ
//    }  
//  
//		DS18B20_Write_Byte(0XCC);				/* ����ROM */
//		DS18B20_Write_Byte(0x44);				/*�����¶�ת��*/
//    DS18B20_Reset();  
//    DS18B20_Write_Byte(0x55);       //ƥ��ROMָ��
//    for(data = 0;data < 8;data ++)   		//����8�ֽڵ����к�
//    {  
//       DS18B20_Write_Byte(pID[data]);  
//    }  
//    Delay_us(10);  
//    DS18B20_Write_Byte(0xBE);    //��ȡ�¶�ָ�� 
//    tl = DS18B20_Read_Byte();    				//��8λ
//    th = DS18B20_Read_Byte();    				//��8λ
//    data = th; 
//		data = data<<8;
//    data = data|tl;  
//		if( data < 0 ) 	/*���¶�*/
//		{
//			f_tem = (~data+1) * 0.0625;
//		}
//		else
//			f_tem = data * 0.0625;
//		return f_tem;	
//}		

float DS18B20_ReadDesignateTemper(uint8_t pID[8])  
{  
	uint8_t temp1, temp2;
 
	short s_tem;
	float f_tem;

	uint8_t i;
	
	DS18B20_Reset();		/* ���߸�λ */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(pID[i]);
	}
	DS18B20_Write_Byte(0x44);	/* ��ת������ */
	DS18B20_Mode_Out_PP();
	DS18B20_DATA_OUT(HIGH); 
	Delay_ms(800);
	DS18B20_Reset();		/* ���߸�λ */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* ������ */
	
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(pID[i]);
	}	
	DS18B20_Write_Byte(0xbe);

	temp1 = DS18B20_Read_Byte();	/* ���¶�ֵ���ֽ� */
	temp2 = DS18B20_Read_Byte();	/* ���¶�ֵ���ֽ� */
	
	DS18B20_Reset();
	DS18B20_Presence();

	s_tem = temp2<<8;
	s_tem = s_tem | temp1;
		if( s_tem < 0 ) 	/*���¶�*/
		{
			f_tem = (~s_tem+1) * 0.0625;
		}
		else
			f_tem = s_tem * 0.0625;
		return f_tem;	
}		

//uint8_t CheckCrc(void)
//{
//	uint8_t crc = 0,i,j;
//	for(i=0;i<sizeof(dat)/sizeof(dat[0]);i++)
//	{
//		crc=crc^dat[i];
//		for(j=0;j<8;j++)
//		{
//			if(crc&0x01)crc=(crc>>1)^0x8c;
//				else crc>>=1;
//		}
//	
//		}
//	return crc;

//}
