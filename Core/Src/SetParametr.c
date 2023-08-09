#include "SetParametr.h"
extern  param p_work;
extern const uint8_t temp_numder[7];
extern uint8_t flag_beg,flag_zumer;
extern uint16_t count_timer;
void SetParametrPlay (void)
{
	uint8_t count_=0;
	int i =read_memory(41);
	while(bt_ok());
	HAL_Delay(1000);
  while(count_<5)
	{
	  count_++;
      lcd_goto(5,0);
      lcd_string("  ");
      HAL_Delay(200);
      lcd_goto(5,0);
      lcd_number(temp_numder[i]);
      HAL_Delay(200);
     if(bt_plus() && i<6)
     {
    	 i++;
    	 count_=0;
     }
     if(bt_minus() && i!=0)
     {
    	 i--;
    	 count_=0;
     }
     }
  if(i!=read_memory(41)) write_memory(41,i);

  p_work.number_cycles= temp_numder[i];
	  count_=0;
    i=p_work.period;

    while(count_<5)
    {
          count_++;
          lcd_goto(7,1);
          lcd_string(" ");
          HAL_Delay(150);
          lcd_goto(7,1);
          lcd_number(i);
          HAL_Delay(150);
         if(bt_plus() && i<9)
            {
        	 i++;
        	 count_=0;
            }
         if(bt_minus() && i!=0)
        	 {
        	   i--;
        	   count_=0;
             }
    }
    if(i!=read_memory(100) ) write_memory(100,i);
    p_work.period=i;
}

void SetMode(void)                              //��������� ������
{
   lcd_goto(0,0);
   lcd_string("HACTP. MODE-");
   while(!(bt_ok()))
   {
	   	if(bt_plus() && p_work.mode<3 ) p_work.mode++;
    	if(bt_minus() && p_work.mode!=0 ) p_work.mode--;
        switch (p_work.mode)
    	{
    	 case game_beg:
		         {
			        lcd_goto(12, 0);
    		        lcd_string("BEG");
    		        break;
    	        }
    	case game_invers:
    			{
    				lcd_goto(12, 0);
    	    		lcd_string("INV");
    	    		break;
    	    	}
    	case game_duble:
    			{
    				if(COUNT_LED!=10)
    				{}
    				else
    				{
    				lcd_goto(12, 0);
    	    		lcd_string("DUB");
    	    		break;
    				}
    	    	}
    	case test:
    			{
    				lcd_goto(12, 0);
    	    		lcd_string("TST");
    	    		break;
    	    	}
    	}
      HAL_Delay(500);
    }
    write_memory (40,p_work.mode);
    lcd_clear();
    indication();
     while((bt_ok()));
   HAL_Delay(300);
}

void  ZumerOk(void)
	{
	   for(int i=0;i<3;i++)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			HAL_Delay(300);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_Delay(300);
	   }
	}
	void ZumerError (void)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	}

void Zumer (void)
{
 uint32_t count=0;
    while(count<100000)
    {
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    	count++;
    }
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
 }

void proverka (uint8_t flag) //���� ������ �������� �� ��������  flag_beg � ���� ���� ����� ������
{
  switch(flag)
  {
     case 1:
    	 if(in_one()!=0)
    	 {
    		 flag_beg=0;
    	 }
    	 break;
     case 2:
    	 if(in_two()!=0)
    		 flag_beg=0;
         break;
     case 3:
    	 if(in_three()!=0)
    		 flag_beg=0;
    	 break;
     case 4:
    	 if(in_four()!=0)
    		 flag_beg=0;
    	 break;
     case 5:
    	 if(in_five()!=0)
    		 flag_beg=0;
    	 break;
     case 6:
    	if(in_six()!=0)
    		flag_beg=0;
    	break;

    case 7:
    	if(in_seven()!=0)
    		flag_beg=0;
    	break;
    case 8:
    	if(in_eight()!=0)
    		flag_beg=0;
    	break;
    case 9:
    	if(in_nine()!=0)
    		flag_beg=0;
    	break;
    case 10:
    	if(in_ten()!=0)
    		flag_beg=0;
    	break;
   }
}


