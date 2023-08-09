#include "SetAdcBotton.h"
extern  struct adc_k button;
 param p_work;
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
            		 write_memory_adc(200,button._minus);
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
            		 write_memory_adc(20,button._plus);
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
	    	            		 write_memory_adc(30,button._reset);
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
	    		    	while(read_adcn()>1010);
}

void indication (void)
{
	lcd_goto(0,0);
	lcd_string("KOLV:");
	lcd_number(p_work.number_cycles);
	lcd_string(" ");
	lcd_goto(0,1);
	lcd_string("TEMP:");
	lcd_string("0,");
	lcd_number(p_work.period);
	lcd_goto(10,1);
	lcd_string("BAL:");
	switch (p_work.mode)
		{
			case 0:
				{
					lcd_goto(12, 0);
					lcd_string("BEG");
					break;
				}
			case 1:
				{
					lcd_goto(12, 0);
					lcd_string("INV");
					break;
				}
			case 2:
				{
					if( COUNT_LED !=10)
					{}
					else
					{
						lcd_goto(12, 0);
						lcd_string("DUB");
						break;
					}

				}
			case 3:
				{
					lcd_goto(12, 0);
					lcd_string("TST");
					break;
				}
			case 4:
				{
					lcd_goto(12, 0);
					lcd_string("MEM");
					break;
				}

		}
}

void RandomNumber (void)
{
	srand(time_random);
	uint8_t out=0,out_old;
  for(uint8_t i=0;i<p_work.number_cycles+1;i++)
  {
     while(out_old==out)
      {
	    out=(rand()%COUNT_LED);
      }
      out_old=out;
     random_numder[i]=out;
  }
}




