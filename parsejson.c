#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "parsejson.h"
#include "malloc.h"
#include "delay.h"

typedef struct
{
	char m_CityName[10];
	char m_Text[10];
	char m_Temp[5];
	char m_Humi[5];
	char m_LastUpdataTime[30];
	char m_WindDir[10];
	char m_WindGrade[8];
	char m_WindSpeed[8];
}Now_Weather;



extern u8 Weather;

extern int SwitchToGbk(const unsigned char* pszBufIn, int nBufInLen, unsigned char* pszBufOut, int* pnBufOutLen);




//解析当前天气
void parse_now_weather(void)
{
	u16 i = 0,j = 0,line = 0;
	int x_1 = 0, x_2 = 0, y_1 = 0, y_2 = 0;
	int len = 0;
	char gbkstr[50] = {0};
	Now_Weather *weather_Structure = mymalloc(SRAMIN,sizeof(Now_Weather));
	for(i = 0; i < strlen((char *)USART3_RX_BUF); i++)
	{
		if(USART3_RX_BUF[i] == 'n' && USART3_RX_BUF[i+1] == 'a' && USART3_RX_BUF[i+2] == 'm' && USART3_RX_BUF[i+3] == 'e')
		{
			while(USART3_RX_BUF[i+7] != '"')
				weather_Structure->m_CityName[j++] = USART3_RX_BUF[7+(i++)];
			weather_Structure->m_CityName[j] = '\0';
			break;
		}
	}
	SwitchToGbk((const u8*)weather_Structure->m_CityName,strlen(weather_Structure->m_CityName),(u8 *)gbkstr,&len);  //获取城市名称转换为gbk文件
	if(strstr(gbkstr,"湛江") != NULL)
	{
		Chinese4_Show_one(120, 20, 0, 16, 0);
		Chinese4_Show_one(140, 20, 1, 16, 0);
	}
	else if(strstr(gbkstr,"惠州") != NULL)  
	{
		Chinese4_Show_one(120, 80, 2, 16, 0);
		Chinese4_Show_one(140, 80, 3, 16, 0);
	}
	
	j = 0;
	for(i = 0; i < strlen((char *)USART3_RX_BUF); i++)
	{
		if(USART3_RX_BUF[i] == 't' && USART3_RX_BUF[i+1] == 'e' && USART3_RX_BUF[i+2] == 'x' && USART3_RX_BUF[i+3] == 't')
		{
			while(USART3_RX_BUF[i+7] != '"')
				weather_Structure->m_Text[j++] = USART3_RX_BUF[7+(i++)];
			weather_Structure->m_Text[j] = '\0';
			break;
		}
	}
	memset(gbkstr,0,sizeof(gbkstr));
	if(Weather == 1)
	{
		line = 40;//湛江
		x_1 = 0;
		x_2 = 80;
		y_1 = 0;
		y_2 = 55;
	}
	else if(Weather == 2)
	{
		line = 100;//惠州
		x_1 = 0;
		x_2 = 80;
		y_1 = 60;
		y_2 = 115;
	}
	SwitchToGbk((const u8*)weather_Structure->m_Text,strlen(weather_Structure->m_Text),(u8 *)gbkstr,&len);
	if(strstr(gbkstr,"阴") != NULL)
	{
		Chinese5_Show_one(150, line, 0, 16, 0);
		Chinese5_Show_one(166, line, 6, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_1);
	}
	else if(strstr(gbkstr,"多云") != NULL)
	{
		Chinese5_Show_one(150, line, 1, 16, 0);
		Chinese5_Show_one(166, line, 2, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_2);
	}
	else if(strstr(gbkstr,"晴") != NULL)
	{
		Chinese5_Show_one(150, line, 3, 16, 0);
		Chinese5_Show_one(166, line, 6, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_0);
	}
	else if(strstr(gbkstr,"小雨") != NULL)
	{
		Chinese5_Show_one(150, line, 4, 16, 0);
		Chinese5_Show_one(166, line, 5, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_4);
	}
	else if(strstr(gbkstr,"中雨") != NULL)
	{
		Chinese5_Show_one(150, line, 4, 16, 0);
		Chinese5_Show_one(166, line, 5, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_4);
	}
	else if(strstr(gbkstr,"大雨") != NULL)
	{
		Chinese5_Show_one(150, line, 4, 16, 0);
		Chinese5_Show_one(166, line, 5, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_4);
	}
	else if(strstr(gbkstr,"雷阵雨") != NULL)
	{
		Chinese5_Show_one(150, line, 4, 16, 0);
		Chinese5_Show_one(166, line, 5, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_4);
	}
	else if(strstr(gbkstr,"阵雨") != NULL)
	{
		Chinese5_Show_one(150, line, 4, 16, 0);
		Chinese5_Show_one(166, line, 5, 16, 0);
		LCD_Draw_Picture(x_1, y_1, x_2, y_2, (u8 *)gImage_4);
	}

	
	j = 0;
	for(i = 0; i < strlen((char *)USART3_RX_BUF); i++)
	{
		if(USART3_RX_BUF[i] == 't' && USART3_RX_BUF[i+1] == 'e' && USART3_RX_BUF[i+2] == 'm' && USART3_RX_BUF[i+3] == 'p')
		{
			while(USART3_RX_BUF[i+14] != '"')
				weather_Structure->m_Temp[j++] = USART3_RX_BUF[14+(i++)];
			weather_Structure->m_Temp[j] = '\0';
			break;
		}
	}
	
	if(Weather == 1) line = 10;//长沙
	else if(Weather == 2) line = 70;//衡阳
	LCD_ShowString(170,line,25,20,24,(u8*)weather_Structure->m_Temp);
	Chinese5_Show_one(200, line+5, 7, 16, 0);
	
	myfree(SRAMIN,weather_Structure);
}







