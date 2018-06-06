/**
  * @file   ds18B20.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   温度获取文件
  */

#include "ds18b20.h"
#include "systemconfig.h"

static float Temperature;/* 获取到的温度 */


/**
  *@brief 配置DS18B20用到的I/O口
  *@param  None
  *@retval None
  */
static void DS18B20_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 															   
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 
}

/**
  *@brief 使DS18B20-DATA引脚变为输入模式
  *@param  None
  *@retval None
  */
static void DS18B20_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/**
  *@brief 使DS18B20-DATA引脚变为输出模式
  *@param  None
  *@retval None
  */
static void DS18B20_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;															   
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/**
  *@brief 主机给从机发送复位脉冲
  *@param  None
  *@retval None
  */
static void DS18B20_Rst(void)
{
	/* 主机设置为推挽输出 */
	DS18B20_Mode_Out_PP();
	DS18B20_DATA_OUT(LOW);
	/* 主机至少产生480us的低电平复位信号 */
	Delay_us(750);
	/* 主机在产生复位信号后，需将总线拉高 */
	DS18B20_DATA_OUT(HIGH);
	/*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
	Delay_us(15);
}

/**
  *@brief 检测从机给主机返回的存在脉冲
  *@param  None
  *@retval 0 成功  1 失败
  */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* 主机设置为上拉输入 */
	DS18B20_Mode_IPU();
	/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	/* 经过100us后，存在脉冲都还没有到来*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* 存在脉冲到来，且存在的时间不能超过240us */
	while( !DS18B20_DATA_IN() && pulse_time < 240 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	if( pulse_time >= 240 )
		return 1;
	else
		return 0;
}

/**
  *@brief 从DS18B20读取一个bit
  *@param  None
  *@retval bit
  */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* 读0和读1的时间至少要大于60us */	
	DS18B20_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	DS18B20_DATA_OUT(LOW);
	Delay_us(10);
	
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	DS18B20_Mode_IPU();
	//Delay_us(2);
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* 这个延时参数请参考时序图 */
	Delay_us(45);
	
	return dat;
}

/**
  *@brief 从DS18B20读一个字节，低位先行
  *@param  None
  *@retval 数据
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

/**
  *@brief 写一个字节到DS18B20，低位先行
  *@param  数据
  *@retval None
  */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			/* 1us < 这个延时 < 15us */
			Delay_us(8);
			
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			Delay_us(70);
			
			DS18B20_DATA_OUT(HIGH);			
			/* 1us < Trec(恢复时间) < 无穷大*/
			Delay_us(2);
		}
	}
}

/**
  *@brief DS18B20开始温度转化
  *@param  None
  *@retval None
  */
void DS18B20_Start(void)
{
	uint8_t waitsuccess = 3;
	while(waitsuccess--){
		DS18B20_Rst();	   
		if(DS18B20_Presence()==0){//检测成功
			DS18B20_Write_Byte(0XCC);		/* 跳过 ROM */
			DS18B20_Write_Byte(0X44);		/* 开始转换 */
			break;//结束循环
		}
	} 	
}

/**
  *@brief 8位CRC校验程序
  *@param  ptr 待校验字符串
  *@param  len 字符串长度
  *@retval 校验值
  */
uint8_t xCal_crc(uint8_t *ptr, uint8_t len)
{
	uint8_t crc;
	uint8_t i;
	crc = 0;
	while (len--)
	{
		crc ^= *ptr++;
		for (i = 0; i < 8; i++)
		{
			if (crc & 0x01)
			{
				crc = (crc >> 1) ^ 0x8C;
			}
			else crc >>= 1;
		}
	}
	return crc;
}
/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
static void DS18B20_Get_Temp(void)
{
	uint8_t i,waitsuccess = 3;
	uint8_t getDsValue[9];
	short s_tem;
	float f_tem;
	while(waitsuccess--){
		DS18B20_Rst();
		if(DS18B20_Presence()==0){
			DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
			DS18B20_Write_Byte(0XBE);				/* 读温度值 */
			for(i=0;i<9;i++){
				getDsValue[i] = DS18B20_Read_Byte();
			}
			if(xCal_crc(getDsValue,8) == getDsValue[8]){
				s_tem = getDsValue[1]<<8;
				s_tem = getDsValue[0] | s_tem;			
				f_tem = s_tem * 0.0625;//温度转化，带正负
				Temperature = f_tem;//将温度给全局变量	
				break;//结束循环
			}
		}		
	}	
}

/**
  *@brief 主函数
  *@param  None
  *@retval 温度 
  */
void Ds18B20Main(void)
{
	static uint32_t tickBegin;/* tick时钟开始值 */
	
	if(GetTimer1IntervalTick(tickBegin) >= CYCTRANSFORMVAULE){/*到达设定时间*/
		DS18B20_Get_Temp();//读取温度
		DS18B20_Start();//温度转换
		tickBegin = GetTimer1Tick();
	}		
}

/**
  *@brief Ds18B20初始化
  *@param  None
  *@retval None
  */
void DS18B20_Init(void)
{
	DS18B20_GPIO_Config();
	Temperature = 0;
	DS18B20_Start();//温度转换
	DS18B20_Get_Temp();//读取温度
}

/**
  *@brief 获取温度，用于外部文件调用
  *@param  None
  *@retval 温度 
  */
float GetTemperature(void)
{
	return Temperature;
}

/* END */
