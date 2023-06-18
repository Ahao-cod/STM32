#ifndef __DS1302_H
#define __DS1302_H

#include "stm32f10x.h"
#include "sys.h"

//���ж������д��ĵ�ַ��д�Ͷ�ֻ����1�����Զ���ַֻ��Ҫ��д��ַ���λ��1����
//DS1302��ַ����
#define DS1302_SEC_ADDR           0x80		//�����ݵ�ַ
#define DS1302_MIN_ADDR           0x82		//�����ݵ�ַ
#define DS1302_HOUR_ADDR          0x84		//ʱ���ݵ�ַ
#define DS1302_DAY_ADDR           0x86		//�����ݵ�ַ
#define DS1302_MONTH_ADDR         0x88		//�����ݵ�ַ
#define DS1302_WEEK_ADDR          0x8a		//�������ݵ�ַ
#define DS1302_YEAR_ADDR          0x8c		//�����ݵ�ַ
#define DS1302_CONTROL_ADDR       0x8e		//�������ݵ�ַ(д������ַ)
#define DS1302_CHARGER_ADDR       0x90 		//��繦�ܵ�ַ			 
#define DS1302_CLKBURST_ADDR      0xbe   


//RST PA5
//CLK PA7
//IO  PA6
#define DS1302_IN  PAin(6)
#define DS1302_OUT PAout(6)
#define DS1302_RST PAout(5)
#define DS1302_CLK PAout(7)

struct TIMEData
{
	u16 year;
	u16  month;
	u16  day;
	u8  hour;
	u8  minute;
	u8  second;
	u8  week;
};

extern struct TIMEData TimeData;
extern u8 readtime[15];
extern char time_buf[8];

void DS1302_Init(void);
void DS1302_IO_Output(void);
void DS1302_IO_Input(void);
void DS1302_WriteByte(u8 addr,u8 data);
u8 DS1302_ReadByte(u8 addr);
void DS1302_WriteTime(char time_buf[8]);
void DS1302_ReadTime(void);
void DS1302_GetTime(void);


#endif

