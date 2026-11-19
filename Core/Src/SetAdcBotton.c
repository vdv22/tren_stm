#include "SetAdcBotton.h"
extern  struct adc_k button;
extern param p_work;
extern uint8_t time_random;
extern uint8_t random_numder[50],rand_number_duble_one[50],rand_number_duble_two[50]; // ������ �� ���������� �������.

void indicanionSet(char *str)
{
	lcd_goto(0,0);
    lcd_string("HACTP.");
	lcd_goto(0,1);
	lcd_string(str);
	HAL_Delay(1000);
}
void Setbotton (void)     // ���������� ������
{
	uint16_t temp,prom;
	indicanionSet("MINUS");
	while(read_adcn()>1010);
	HAL_Delay(1500);
	while(1)
	{
		temp=read_adcn();
        if(temp<1010 && temp>10)
        {
        	if(prom==0)
        	{
        		HAL_Delay(500);
            	button._minus=read_adcn();
            	lcd_goto(7,1);
            	lcd_string("ok");
            	prom=1;
            }
        }
        	if(temp>1010 && prom==1 )
            {
        	   lcd_goto(7,1);
        	   lcd_string("  ");
        	   prom=0;
        	   break;
            }
               HAL_Delay(100);
	}

	lcd_goto(0,1);
	lcd_string("PLUS.  ");
	HAL_Delay(1000);
	while(read_adcn()>1010);
	HAL_Delay(500);
	while(1)
	{
		temp=read_adcn();
        if(temp<1010 && temp>10)
        {
        	if(prom==0)
        	{
        		HAL_Delay(500);
        		button._plus=read_adcn();
        		lcd_goto(7,1);
            	lcd_string("ok");
            	prom=1;
        	}
        }
            if(temp>1010 && prom==1 )
            {
            	lcd_goto(7,1);
        	    lcd_string("  ");
        	    prom=0;
        	    break;
            }
	}
	lcd_goto(0,1);
	lcd_string("RESET");
	HAL_Delay(1000);
	while(read_adcn()>1010);
	HAL_Delay(500);
	while(1)
	{
		temp=read_adcn();
	    	                if(temp<1010 && temp>10)
	    	              {
	    	            	  if(prom==0)
	    	            	  {
	    	            		 HAL_Delay(500);
	    	            		 button._reset=read_adcn();
	    	            		 lcd_goto(7,1);
	    	            		 lcd_string("ok");
	    	            		 prom=1;
	    	            	 }
	    	             }
	    	                if(temp>1010 && prom==1 )
	    	                   {
	    	        	         lcd_goto(7,1);
	    	        	         lcd_string("  ");
	    	        	         prom=1;
	    	        	         break;
	    	                  }
	    		    	}
	    		    	HAL_Delay(300);
	    		     int_mem_write();
}

void indication (void)
{
	lcd_goto(0,0);
	lcd_string("KOLV:");
	lcd_number(p_work.number_cycles);
	lcd_string(" ");
	lcd_goto(0,1);
	lcd_string("TEMP:");
	lcd_number(p_work.period / 10);
	lcd_string(",");
	lcd_number(p_work.period % 10);
	lcd_string(" ");

	lcd_goto(10,1);
	lcd_string("BAL:");
	switch (p_work.mode)
		{
			case game_beg:
				lcd_goto(12, 0);
				lcd_string("BEG ");
				break;
			case game_duble:
				lcd_goto(12, 0);
				lcd_string("BEG2");
				break;
			case game_beg3:
				lcd_goto(12, 0);
				lcd_string("BEG3");
				break;
			case game_beg4:
				lcd_goto(12, 0);
				lcd_string("BEG4");
				break;
			case game_invers:
				lcd_goto(12, 0);
				lcd_string("INV ");
				break;
		    case game_inv_double:
		    	lcd_goto(12, 0);
		    	lcd_string("INV2");
		    	break;
		    case game_inv3:
		    	lcd_goto(12, 0);
		    	lcd_string("INV3");
		    	break;
		    case game_inv4:
		    	lcd_goto(12, 0);
		    	lcd_string("INV4");
		    	break;
		    case mem_game:
		    	lcd_goto(12, 0);
		    	lcd_string("MEM ");
		    	break;
		    case game_mem_double:
		    	lcd_goto(12, 0);
		    	lcd_string("MEM2");
		    	break;
		    case game_color_pair:
		    	lcd_goto(12, 0);
		    	lcd_string("DVA ");
		    	break;
		    case test:
				lcd_goto(12, 0);
				lcd_string("TST ");
				break;
		}
}

void RandomNumber (void)
{
	srand(time_random + (uint8_t)HAL_GetTick());
	uint8_t out=0,out_old=255;
  for(uint8_t i=0;i<p_work.number_cycles+1;i++)
  {
     while(out_old==out)
      {
	    out=(rand()%COUNT_LED);
      }
      out_old=out;
     random_numder[i]=out;
  }
  /* Гарантируем, что первая мишень раунда не равна мишени №1 (0),
     чтобы отпускающий палец не вызвал ложного срабатывания */
  if (random_numder[1] == 0)
  {
      random_numder[1] = 1 + (rand() % (COUNT_LED - 1));
  }
}
