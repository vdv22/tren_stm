#include "doubleColor.h"
extern  param p_work;
extern uint16_t count_timer;
extern uint8_t  step;



void memory_game (void)
{

     if (step==1)
     {
    	 memory_game_led_on();
     }

     if(step==2)
     {
    	 CheckMem();
     }


}

