#ifndef __TIM3_H
#define __TIM3_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "gpio.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);

#endif

