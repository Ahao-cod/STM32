#ifndef __DS1302_H
#define __DS1302_H

#include "stm32f10x.h"
#include "sys.h"

//下列定义的是写入的地址，写和读只差了1，所以读地址只需要把写地址最低位置1即可
//DS1302地址定义
#define DS1302_SEC_ADDR           0x80		//秒数据地址
#define DS1302_MIN_ADDR           0x82		//分数据地址
#define DS1302_HOUR_ADDR          0x84		//时数据地址
#define DS1302_DAY_ADDR           0x86		//日数据地址
#define DS1302_MONTH_ADDR         0x88		//月数据地址
#define DS1302_WEEK_ADDR          0x8a		//星期数据地址
#define DS1302_YEAR_ADDR          0x8c		//年数据地址
#define DS1302_CONTROL_ADDR       0x8e		//控制数据地址(写保护地址)
#define DS1302_CHARGER_ADDR       0x90 		//充电功能地址			 
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

