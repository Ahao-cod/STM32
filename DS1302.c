#include "DS1302.h"
#include "delay.h"


//RST PA5
//CLK PA7
//IO  PA6

//初始时间定义
char time_buf[8] = {0x20,0x23,3,27,10,10,00,01};//初始时间
u8 readtime[15];//当前时间
u8 sec_buf=0;  //秒缓存
u8 sec_flag=0; //秒标志位

struct TIMEData TimeData;



//根据时序图，初始化这两个引脚的时候要拉低，所以初始化的时候就顺便拉低
void DS1302_Init()
{
	GPIO_InitTypeDef DS1302;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	DS1302.GPIO_Pin = GPIO_Pin_5; 				//RST
	DS1302.GPIO_Speed = GPIO_Speed_50MHz;
	DS1302.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(GPIOA, &DS1302);			
	GPIO_ResetBits(GPIOA,GPIO_Pin_5); 
	
	DS1302.GPIO_Pin = GPIO_Pin_7; 				//CLK
	DS1302.GPIO_Speed = GPIO_Speed_50MHz;
	DS1302.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(GPIOA, &DS1302);			
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	
}

//配置模块的IO口PA6，要配置输入和输出
void DS1302_IO_Output()
{
	GPIO_InitTypeDef DS1302_IO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	DS1302_IO.GPIO_Pin = GPIO_Pin_6; 				//IO
	DS1302_IO.GPIO_Speed = GPIO_Speed_50MHz;
	DS1302_IO.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(GPIOA, &DS1302_IO);			
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}

void DS1302_IO_Input()
{
	GPIO_InitTypeDef DS1302_IO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	DS1302_IO.GPIO_Pin = GPIO_Pin_6;              //IO
	DS1302_IO.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_Init(GPIOA, &DS1302_IO);
}



//向DS1302写入一字节数据
void DS1302_WriteByte(u8 addr,u8 data) 
{
	u8 i;
	DS1302_RST=0; //禁止数据传输 ！！！这条很重要
	DS1302_CLK=0; //确保写数据前SCLK为低电平
	DS1302_RST=1;	//启动DS1302总线	
	DS1302_IO_Output();
	addr=addr&0xFE;  //最低位置零，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) //写入目标地址：addr
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //时钟上升沿写入数据
		DS1302_CLK=0;
		addr=addr>>1;
	}	
	for (i=0;i<8;i++) //写入数据：data
	{
		if(data&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1;    //时钟上升沿写入数据
		DS1302_CLK=0;
		data = data >> 1;
	}
	DS1302_CLK=1;    // 将时钟电平置于高电平状态 ，处于已知状态
	DS1302_RST=0;	//停止DS1302总线
}


//从DS1302读出一字节数据
u8 DS1302_ReadByte(u8 addr) 
{
	u8 i,temp;	
	DS1302_RST=0; 			//这条很重要
	DS1302_CLK=0; 			//先将SCLK置低电平,确保写数居前SCLK被拉低
	DS1302_RST=1; 			//启动DS1302总线
	DS1302_IO_Output();
	//写入目标地址：addr
	addr=addr|0x01; //最低位置高，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) 
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //写数据
		DS1302_CLK=0;
		addr = addr >> 1;
	}	
	//从DS1302读出数据：temp
	DS1302_IO_Input();
	for(i=0;i<8;i++)
	{
		temp=temp>>1;
		if (DS1302_IN) temp|=0x80;
		else temp&=0x7F;
		DS1302_CLK=1;
		DS1302_CLK=0;
	}	
	DS1302_CLK=1;  //将时钟电平置于已知状态
	DS1302_RST=0;	//停止DS1302总线
	return temp;
}

//时间写入
//写入也要转换成BCD码写入，否则无法正确识别
void DS1302_WriteTime(char time_buf[8]) 
{
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x00);       //关闭写保护 
	DS1302_WriteByte(DS1302_YEAR_ADDR,time_buf[1]);   //年 
	DS1302_WriteByte(DS1302_MONTH_ADDR,time_buf[2]/10*16 + time_buf[2]%10);  //月 
	DS1302_WriteByte(DS1302_DAY_ADDR,time_buf[3]/10*16 + time_buf[3]%10);    //日 
	DS1302_WriteByte(DS1302_HOUR_ADDR,time_buf[4]/10*16 + time_buf[4]%10);   //时 
	DS1302_WriteByte(DS1302_MIN_ADDR,time_buf[5]/10*16 + time_buf[5]%10);    //分
	DS1302_WriteByte(DS1302_SEC_ADDR,time_buf[6]/10*16 + time_buf[6]%10);    //秒
	DS1302_WriteByte(DS1302_WEEK_ADDR,time_buf[7]/10*16 + time_buf[7]%10);	  //周 
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x80);			//打开写保护  
	
}

//时间读出
void DS1302_ReadTime()  
{
	time_buf[1]=DS1302_ReadByte(DS1302_YEAR_ADDR);          //年 
	time_buf[2]=DS1302_ReadByte(DS1302_MONTH_ADDR);         //月 
	time_buf[3]=DS1302_ReadByte(DS1302_DAY_ADDR);           //日 
	time_buf[4]=DS1302_ReadByte(DS1302_HOUR_ADDR);          //时 
	time_buf[5]=DS1302_ReadByte(DS1302_MIN_ADDR);           //分 
	time_buf[6]=DS1302_ReadByte(DS1302_SEC_ADDR);    //秒，屏蔽秒的第7位，避免超出59
	time_buf[7]=DS1302_ReadByte(DS1302_WEEK_ADDR);          //周 	
}


//DS1302获取完时间后进行数据的计算
//读出来的是8421码，转换后才能使用
void DS1302_GetTime()
{ 
    DS1302_ReadTime(); //读取时间
    TimeData.year	=(time_buf[0]>>4)*1000	+(time_buf[0]&0x0F)*100+(time_buf[1]>>4)*10+(time_buf[1]&0x0F); //计算年份
    TimeData.month	=time_buf[2]/16*10	+time_buf[2]%16;  	//计算月份
    TimeData.day	=time_buf[3]/16*10	+time_buf[3]%16;    //计算日期
    TimeData.hour	=time_buf[4]/16*10	+time_buf[4]%16;   	//计算小时
    TimeData.minute	=time_buf[5]/16*10	+time_buf[5]%16; 	//计算分钟
    TimeData.second	=time_buf[6]/16*10	+time_buf[6]%16; 	//计算秒钟
    TimeData.week	=time_buf[7]/16*10	+time_buf[7]%16;                      		//计算星期																										
}




