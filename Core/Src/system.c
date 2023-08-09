#include "system.h"

u8 chekInputBotton (GPIO_TypeDef *g,uint16_t Pin)
{
	if(READ_BIT(g->IDR,0x1UL << Pin )==RESET)
		return 1;
	else
		return 0;
}

u8 debonse(u8 count,GPIO_TypeDef *g,uint16_t pin)
{
	u8 c=0,on=0,off=0;
	for(c=0;c<count;c++)
	{
		if((g->IDR&pin)==RESET)
		  on++;
		else
		  off++;
		HAL_Delay(1);
	}
	if (on>off) return 1; else return 0;
}


