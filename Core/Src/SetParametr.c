#include "SetParametr.h"
extern  param p_work;
extern const uint8_t temp_numder[6];
extern uint8_t flag_beg,flag_zumer;
extern uint16_t count_timer;
extern uint8_t position;
extern uint8_t start_play;
void SetParametrPlay (void)          //set  speed and quantity
{
	uint8_t count_= 0;
	int i = position;
	if (i >= 6) i = 0;
	while(bt_ok());
	HAL_Delay(500);
  while(count_<5)
	{
	  count_++;
      lcd_goto(5,0);
      lcd_string("  ");
      HAL_Delay(150);
      lcd_goto(5,0);
      lcd_number(temp_numder[i]);
      HAL_Delay(150);
     if(bt_plus() && i<5)
     {
    	 i++;
    	 count_=0;
    	 HAL_Delay(200);
     }
     if(bt_minus() && i!=0)
     {
    	 i--;
    	 count_=0;
    	 HAL_Delay(200);
     }
     }

  p_work.number_cycles= temp_numder[i];
  position=i;
	  count_=0;
      i=p_work.period;

    while(count_<5)
    {
          count_++;
          lcd_goto(5,1);
          lcd_string("   ");
          HAL_Delay(150);
          lcd_goto(5,1);
          if( i<10  )
          {
        	  lcd_number(0);
        	  lcd_string(",");
        	  lcd_number(i);
          }
          else if (i>=10 && i!=20)
          {
        	  lcd_number(1);
        	  lcd_string(",");
        	  lcd_number(i%10);
          }
          else
          {
               lcd_string("2,0");
          }


          HAL_Delay(150);



         if(bt_plus() && i<10)
            {
        	 i++;
        	 count_=0;
        	 HAL_Delay(150);
            }
         else if (bt_plus() && i>=10 && i<20)
         {
        	 i+=5;
        	 count_=0;
        	 HAL_Delay(150);
         }
/////////////////////////////////////
         if(bt_minus() && i!=0  && i<=10)
        	 {
        	   i--;
        	   count_=0;
        	   HAL_Delay(150);
             }
         else if ( bt_minus() && i>10  )
         {
        	 i-=5;
        	 count_=0;
        	 HAL_Delay(150);
         }

    }
    p_work.period=i;
    int_mem_write();
    lcd_clear();
    indication();
}

void SetMode(void)                              //Set mode GAME
{
   while(bt_reset());
   HAL_Delay(200);

   lcd_goto(0,0);
   lcd_string("HACTP. MODE-");
   lcd_goto(0,1);
   lcd_string("                ");

   while(!(bt_ok()))
   {
	   	if(bt_plus() && p_work.mode<11 )
	   	{
	   		p_work.mode++;
	   		HAL_Delay(400);
	   	}
    	if(bt_minus() && p_work.mode!=0 )
    	{
    		p_work.mode--;
    		HAL_Delay(400);
    	}
        switch (p_work.mode)
    	{
    	case game_beg:        lcd_goto(12, 0); lcd_string("BEG "); break;
    	case game_duble:      lcd_goto(12, 0); lcd_string("BEG2"); break;
    	case game_beg3:       lcd_goto(12, 0); lcd_string("BEG3"); break;
    	case game_beg4:       lcd_goto(12, 0); lcd_string("BEG4"); break;
    	case game_invers:     lcd_goto(12, 0); lcd_string("INV "); break;
        case game_inv_double: lcd_goto(12, 0); lcd_string("INV2"); break;
        case game_inv3:       lcd_goto(12, 0); lcd_string("INV3"); break;
        case game_inv4:       lcd_goto(12, 0); lcd_string("INV4"); break;
        case mem_game:        lcd_goto(12, 0); lcd_string("MEM "); break;
        case game_mem_double: lcd_goto(12, 0); lcd_string("MEM2"); break;
        case game_color_pair: lcd_goto(12, 0); lcd_string("DVA "); break;
        case test:            lcd_goto(12, 0); lcd_string("TST "); break;
    	}
      HAL_Delay(100);
    }
   int_mem_write();
    lcd_clear();
    indication();
    while((bt_ok()));
   HAL_Delay(300);
   on_one();
   start_play = 0;
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
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    for (volatile uint32_t count = 0; count < 100000; count++)
    {
        __NOP();
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
