#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"
#include "sys.h"

//相关参数设置
int count = 0;
int count1 = 0;
int key_count = 0;
int mode = 0;
int set = 2;
int flag = 0;

//温度参数
u8 Temp = 0;
u8 Humi = 0;

//模式设置函数(显示模式与时间设置模式)
void Mode_Set()
{
	if(key[0].single_flag == 1)
	{
		mode = mode + 1;
		if(mode > 1)
		{
			mode = 0;
		}
		key[0].single_flag = 0;
	}
}

//时间设置函数
void Time_Set()
{
	if(key[1].single_flag == 1)
	{
		set = set + 1;
		if(set > 7)
		{
			set = 2;
			flag = 1;
		}
		key[1].single_flag = 0;
	}
	if(key[2].single_flag == 1)
	{
		if(set == 2)
		{
			TimeData.month = TimeData.month + 1;
			if(TimeData.month > 12)
			{
				TimeData.month = 1;
			}
			time_buf[2] = TimeData.month;
		}
		if(set == 3)
		{
			TimeData.day = TimeData.day + 1;
			if(TimeData.day > 31)
			{
				TimeData.day = 1;
			}
			time_buf[3] = TimeData.day;
		}
		if(set == 4)
		{
			TimeData.hour = TimeData.hour + 1;
			if(TimeData.hour > 24)
			{
				TimeData.hour = 0;
			}
			time_buf[4] = TimeData.hour;
		}
		if(set == 5)
		{
			TimeData.minute = TimeData.minute + 1;
			if(TimeData.minute > 60)
			{
				TimeData.minute = 0;
			}
			time_buf[5] = TimeData.minute;
		}
		if(set == 6)
		{
			TimeData.second = TimeData.second + 1;
			if(TimeData.second > 60)
			{
				TimeData.second = 0;
			}
			time_buf[6] = TimeData.second;
		}
		if(set == 7)
		{
			TimeData.week = TimeData.week + 1;
			if(TimeData.week > 7)
			{
				TimeData.week = 1;
			}
			time_buf[7] = TimeData.week;
		}
		key[2].single_flag = 0;
	}
	if(flag == 1)
	{
		DS1302_Init();
	  DS1302_WriteTime(time_buf);
		flag = 0;
	}
}

//主函数
 int main(void)
 {	
	 delay_init();
	 IO_init();
	 Key_Init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 //串口初始化115200
	 uart_init(115200);
	 //初始化时钟模块完成并写入一次时间
	 DS1302_Init();
	 DS1302_WriteTime(time_buf);
	 //5ms中断
	 TIM3_Int_Init(49,7199);        
	 LCD_Init();
	 //初始化温湿度传感器完成并读一次数据
	 DHT11_Init();
	 DHT11_Read_Data(&Temp, &Humi);
	 POINT_COLOR=RED;
	 //初始化串口3与8266通信
	 usart3_init(115200); 
	 //初始化内部内存池
	 my_mem_init(SRAMIN);
	 //8266初始化
	 atk_8266_wifista_config();  
	 delay_ms(210);
	 get_current_weather();
	 delay_ms(80);
	 get_current_weather();
	 delay_ms(80);
		while(1) 
		{
			Key_Control();
			Mode_Set();
			if(mode == 0)
			{
				DS1302_GetTime();
				if(count > 100)    //500ms更新一次室内温湿度
				{
					DHT11_Read_Data(&Temp, &Humi);
					count = 0;
				}
				if(count1 > 6000)  //30s更新一次网络天气 
				{
					get_current_weather();
					count1 = 0;
				}
				LCD_Dis_Init();
				Time_Show();
				Week_Control(TimeData.week);
			}
			if(mode == 1)
			{
				Time_Set();
				Time_Set_Show();
			}
			
		}
	
 }
 
