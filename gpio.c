#include "gpio.h"
#include "sys.h"

struct KEY key[3] = {0};


void IO_init()
{
		//定义GPIO结构体
		GPIO_InitTypeDef  GPIO_InitStructure;
		 
		//开启GPIO相应时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|
		RCC_APB2Periph_GPIOE, ENABLE);	  
		 
		//GPIO引脚初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			    
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
		GPIO_Init(GPIOB, &GPIO_InitStructure);		
		GPIO_SetBits(GPIOB,GPIO_Pin_5);		
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	            
		GPIO_Init(GPIOE, &GPIO_InitStructure);	  	       
		GPIO_SetBits(GPIOE,GPIO_Pin_5); 
}

void Key_Init()
{
	GPIO_InitTypeDef  KEYInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	KEYInit.GPIO_Pin = GPIO_Pin_4;
	KEYInit.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &KEYInit);
	
	KEYInit.GPIO_Pin = GPIO_Pin_3;
	KEYInit.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &KEYInit);
	
  KEYInit.GPIO_Pin = GPIO_Pin_2;
	KEYInit.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &KEYInit);
	
}

void Key_Control()
{
	int i = 0;
	if(key_count > 2)
	{
		key_count = 0;
		
		key[0].key_sta = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
		key[1].key_sta = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);
		key[2].key_sta = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);
		
		for(i = 0;i < 3;i++)
		{
			switch (key[i].judeg_sta)
			{
				case 0:
				{
					if(key[i].key_sta == 0)
					{
						key[i].judeg_sta = 1;
					}
				}
				break;
				
				case 1:
				{
					if(key[i].key_sta == 0)
					{
						key[i].judeg_sta = 2;
					}
					else
					{
						key[i].judeg_sta = 0;
					}
				}
				break;
				
				case 2:
				{
					if(key[i].key_sta == 1)
					{
						key[i].single_flag = 1;
						key[i].judeg_sta = 0;
					}
				}
				break;
			}
		}
	}
}



