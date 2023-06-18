#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

void IO_init(void);
void Key_Init(void);
void Key_Control(void);

#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5

struct KEY
{
	int judeg_sta;
	char key_sta;
	char single_flag;
};

extern struct KEY key[3];

#endif


