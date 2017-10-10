/***********************************************************************
文件名称：DS18B20.C
功    能：完成对对DS18B20的基本操作
编写时间：2017.3.15
编 写 人：
注    意：
***********************************************************************/
#include "main.h"	

unsigned char Temp_Flag=0;
uint8_t g_rom_id[8] = {0};
uint8_t Temp_Hex = 0;
uint8_t ID_Buff[MAXNUM][8];
/*********************************************************************************************************
*	函 数 名: bsp_InitDS18B20
*	功能说明: 配置STM32的GPIO和SPI接口，用于连接 DS18B20
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/


/*********************************************************************************************************
*	函 数 名: bsp_InitDS18B20
*	功能说明: 配置STM32的GPIO和SPI接口，用于连接 DS18B20
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: DS18B20_Mode_Out_PP
*	功能说明: 配置输出
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: DS18B20_Reset
*	功能说明: 复位DS18B20。 拉低DQ为低，持续最少480us，然后等待
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

static void  DS18B20_Reset(void)
{
	DS18B20_Mode_Out_PP();	//主机设置为推挽输出
	DS18B20_DATA_OUT(LOW);	//主机至少产生480us的低电平复位信号
	Delay_us(750);	//延迟750us
	DS18B20_DATA_OUT(HIGH);	//主机产生复位信号之后，需将总线拉高，最少延时15us
	Delay_us(15);	//延迟15us后，等待总线被DS18B20拉低
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_Presence
*	功能说明: 监测总线是否被DS18B20拉低
*	形    参: 无
*	返 回 值: 0:成功，1:失败
*********************************************************************************************************
*/
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
		
	
	DS18B20_Mode_IPU();	//主机设置为上拉输入
	/*等待低脉冲的到来，存在一个60~240us的低电平信号，如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后发给主机一个低电平脉冲*/	
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
*	函 数 名: DS18B20_Read_Bit
*	功能说明: 读取DS18B20总线一位数据
*	形    参: 无
*	返 回 值: dat
*********************************************************************************************************
*/

static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	DS18B20_Mode_Out_PP();	//读0和1的时间至少要大于60us
	DS18B20_DATA_OUT(LOW);
	Delay_us(10);	//主机产生一个1~15us的低电平信号
	
	DS18B20_Mode_IPU();	//设置成输出，释放总线，由外部上拉电阻将总线拉高
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	Delay_us(45);
	
	return dat;
}



/*
*********************************************************************************************************
*	函 数 名: DS18B20_Read_Byte
*	功能说明: 向DS18B20读取1字节数据
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: DS18B20_Read2Bit
*	功能说明: 向DS18B20读取2个位
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: DS18B20_WriteByte
*	功能说明: 向DS18B20写入1字节数据
*	形    参: 无
*	返 回 值: 无
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
	/*写0和1的时间至少要大于60us*/
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			Delay_us(8);	//这个延时1~15us
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);
			
			Delay_us(70);	//这个延时 60us <  0 < 120us 
				
			DS18B20_DATA_OUT(HIGH); 		
			
			Delay_us(2);	//回复时间 1us<Trec<无穷大
		}
	}

}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_WriteByte
*	功能说明: 向DS18B20写入1字节数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DS18B20_Write_Bit(uint8_t dat)
{

	DS18B20_Mode_Out_PP();
	
		if (dat)
		{			
			DS18B20_DATA_OUT(LOW);
			Delay_us(8);	//这个延时1~15us
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);
			
			Delay_us(70);	//这个延时 60us <  0 < 120us 
				
			DS18B20_DATA_OUT(HIGH); 		
			
			Delay_us(2);	//回复时间 1us<Trec<无穷大
		}

}

void DS18B20_Start(void)
{
	DS18B20_Reset();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* 跳过ROM */
	DS18B20_Write_Byte(0X44);		/* 开始转换*/
}

uint8_t DS18B20_Init(void)
{
	DS18B20_GPIO_Configuration();
	DS18B20_Reset();
	
	return DS18B20_Presence();
}




/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadTempReg
*	功能说明: 读温度寄存器的值（原始数据）
*	形    参: 无
*	返 回 值: 温度寄存器数据 （除以16得到 1摄氏度单位, 也就是小数点前面的数字)
*********************************************************************************************************
*/
float DS18B20_ReadTempReg(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Reset();	//复位DS18B20   
	DS18B20_Presence();	//等待低电平时序
	DS18B20_Write_Byte(0XCC);				/* 跳过ROM */
	DS18B20_Write_Byte(0X44);				/* 开始转换*/
	
	DS18B20_Reset();
  DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* 跳过ROM */
  DS18B20_Write_Byte(0XBE);				/*读取温度值*/
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/*负温度*/
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadID
*	功能说明: 读DS18B20的ROM ID， 总线上必须只有1个芯片
*	形    参: _id 存储ID
*	返 回 值: 0 表示失败， 1表示检测到正确ID
*********************************************************************************************************
*/
uint8_t DS18B20_ReadID(uint8_t *_id)
{
	uint8_t i;

	/* 总线复位 */
	DS18B20_Reset();
	if (DS18B20_Presence() == 1)
	{
		return 0;
	}

	DS18B20_Write_Byte(0x33);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		_id[i] = DS18B20_Read_Byte();
	}

	DS18B20_Reset();		/* 总线复位 */
	DS18B20_Presence();
	
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadTempByID
*	功能说明: 读指定ID的温度寄存器的值（原始数据）
*	形    参: 无
*	返 回 值: 温度寄存器数据 （除以16得到 1摄氏度单位, 也就是小数点前面的数字)
*********************************************************************************************************
*/
float DS18B20_ReadTempByID(uint8_t *_id)
{
	uint8_t temp1, temp2;
 
	short s_tem;
	float f_tem;

	uint8_t i;
	
	DS18B20_Reset();		/* 总线复位 */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(_id[i]);
	}
	DS18B20_Write_Byte(0x44);	/* 发转换命令 */
	DS18B20_Mode_Out_PP();
	DS18B20_DATA_OUT(HIGH); 
	Delay_ms(800);
	DS18B20_Reset();		/* 总线复位 */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* 发命令 */
	
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(_id[i]);
	}	
	DS18B20_Write_Byte(0xbe);

	temp1 = DS18B20_Read_Byte();	/* 读温度值低字节 */
	temp2 = DS18B20_Read_Byte();	/* 读温度值高字节 */
	
	DS18B20_Reset();
	DS18B20_Presence();

	s_tem = temp2<<8;
	s_tem = s_tem | temp1;
		if( s_tem < 0 ) 	/*负温度*/
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
*	函数名称：unsigned char Read_ROM_ID(unsigned char * data)
*	功    能：读1820的ROM id 返回1则读成功，id在data所指位置，返回0失败，将data指定地址清0
*	形    参: *data
*	返 回 值: 读取成功返回1，否则返回0
***********************************************************************
*/
unsigned char Read_ROM_ID(unsigned char * data)
{
	/* 读 ROM ID */
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
*	函数名称：unsigned char Read_ROM_ID(unsigned char * data)
*	功    能：读1820的ROM id 返回1则读成功，id在data所指位置，返回0失败，将data指定地址清0
*	形    参: *data
*	返 回 值: 读取成功返回1，否则返回0
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
        DS18B20_Reset();								//复位DS18B20总线
				Delay_us(480);
        DS18B20_Write_Byte(0xF0);	//搜索ROM  
        for(m=0;m<8;m++)  
        {  
            unsigned char s=0;  
            for(n=0;n<8;n++)  
            {  
                k=DS18B20_Read2Bit();		// 读取两位数据
                k=k&0x03;  
                s= s>>1;  
                if(k==0x01)							//0000 0001 如果读到的数据为0
                {             
                    DS18B20_Write_Bit(0);//写0，使总线上为0的器件相应
                    ss[(m*8+n)]=0;  
                }  
                else if(k==0x02)				//0000 0010 如果读到的数据为1
                {  
                    s=s|0x80;  
                    DS18B20_Write_Bit(1);//写1，使总线上为1的器件相应
                    ss[(m*8+n)]=1;  
                }  
                else if(k==0x00)//如果读取到的数据为00，则有冲突，需要进行冲突位判定
                {                
                    ConflictBit=m*8+n+1;                   
                    if(ConflictBit>BUFFER[l])//如果冲突位大于栈顶，则写0
                    {                         
                        DS18B20_Write_Bit(0);  
                        ss[(m*8+n)]=0;                                                
                        BUFFER[++l]=ConflictBit;                         
                    }  
                    else if(ConflictBit<BUFFER[l])//如果冲突位小于栈顶，则写以前的数据
                    {  
                        s=s|((ss[(m*8+n)]&0x01)<<7);  
                        DS18B20_Write_Bit(ss[(m*8+n)]);  
                    }  
                    else if(ConflictBit==BUFFER[l])//如果冲突位等于栈顶，则写1
                    {  
                        s=s|0x80;  
                        DS18B20_Write_Bit(1);  
                        ss[(m*8+n)]=1;  
                        l=l-1;  
                    }  
                }  
                else//如果读到的数据为0x03，则说明单总线上不存在任何设备
                {  
                    return num; //搜索完成，返回搜索到的个数
                }
								Delay_us(5);
            }  
            pID[num][m]=s;
						s=0;
        }  
        num=num+1;
    }  
    while(BUFFER[l]!=0&&(num<MAXNUM));   
      
    return num;     //返回搜索到的个数
}


/*
**********************************************************************
*	函数名称:S18B20_ReadDesignateTemper
*	功    能：读1820的ROM id 返回1则读成功，id在data所指位置，返回0失败，将data指定地址清0
*	形    参: *data
*	返 回 值: 读取成功返回1，否则返回0
***********************************************************************
*/   

//float DS18B20_ReadDesignateTemper(uint8_t pID[8])  
//{  
//    uint8_t th, tl;  
//    uint16_t data;  
//		float f_tem;
//		DS18B20_Reset();
//      
//    if(DS18B20_Presence() == 1)				//错误判断   
//    {  
//        return 0xffff;              		//返回错误信息
//    }  
//  
//		DS18B20_Write_Byte(0XCC);				/* 跳过ROM */
//		DS18B20_Write_Byte(0x44);				/*启动温度转换*/
//    DS18B20_Reset();  
//    DS18B20_Write_Byte(0x55);       //匹配ROM指令
//    for(data = 0;data < 8;data ++)   		//发送8字节的序列号
//    {  
//       DS18B20_Write_Byte(pID[data]);  
//    }  
//    Delay_us(10);  
//    DS18B20_Write_Byte(0xBE);    //读取温度指令 
//    tl = DS18B20_Read_Byte();    				//低8位
//    th = DS18B20_Read_Byte();    				//高8位
//    data = th; 
//		data = data<<8;
//    data = data|tl;  
//		if( data < 0 ) 	/*负温度*/
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
	
	DS18B20_Reset();		/* 总线复位 */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(pID[i]);
	}
	DS18B20_Write_Byte(0x44);	/* 发转换命令 */
	DS18B20_Mode_Out_PP();
	DS18B20_DATA_OUT(HIGH); 
	Delay_ms(800);
	DS18B20_Reset();		/* 总线复位 */
	DS18B20_Presence();
	DS18B20_Write_Byte(0x55);	/* 发命令 */
	
	for (i = 0; i < 8; i++)
	{
		DS18B20_Write_Byte(pID[i]);
	}	
	DS18B20_Write_Byte(0xbe);

	temp1 = DS18B20_Read_Byte();	/* 读温度值低字节 */
	temp2 = DS18B20_Read_Byte();	/* 读温度值高字节 */
	
	DS18B20_Reset();
	DS18B20_Presence();

	s_tem = temp2<<8;
	s_tem = s_tem | temp1;
		if( s_tem < 0 ) 	/*负温度*/
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
