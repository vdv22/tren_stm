#include "inverse.h"
extern uint16_t count_timer;
extern  param p_work;
extern uint8_t start_play,counter_number,random_numder[50],flag_beg,count_zumer,flag_zumer,count_mem,rand_number_duble_one[50],rand_number_duble_two[50];
void invert (void)
{
      if ((p_work.timer_game >= p_work.period))
	  {
    	  p_work.timer_game = 0;
    	  counter_number++;
	      if (COUNT_LED == 10)
	      {
	    	  off_all_invert();
	      }
	      else
	      {
	    	  off_all_invert_half();
	      }
      }

      if (counter_number <= p_work.number_cycles)
	  {
	    	count_timer = 0;
	    	if (flag_beg == 0)
	    	{
	    		onLedInvert(random_numder[counter_number]);
	    	}
	  }
	  else
	  {
	    	off_all();
	    	start_play = 2;
	    	flag_zumer = 1;
	  }
	  if (flag_beg == 0) p_work.timer_game++;
}
void onLedInvert(uint8_t on)
{
		switch(on)
		{
			case 0:
				on_one_invert();
			    break;
			case 1:
				on_two_invert();
			    break;
			case 2:
				on_three_invert();
			    break;
			case 3:
				on_four_invert();
			    break;
			case 4:
				on_five_invert();
			    break;
			case 5:
				on_six_invert();
			    break;
			case 6:
				on_seven_invert();
			    break;
			case 7:
				on_eight_invert();
			    break;
			case 8:
				on_nine_invert();
			    break;
			case 9:
				on_ten_invert();
			    break;
		}
}

static uint8_t inv_scored_cycle = 255;

void reset_inv_scored_cycle(void)
{
    inv_scored_cycle = 255;
}

void ifBottonOnInvert (uint8_t n)
{
    if (inv_scored_cycle != counter_number && counter_number <= p_work.number_cycles)
    {
        inv_scored_cycle = counter_number;
        if (p_work.bal < p_work.number_cycles)
        {
            p_work.bal++;
        }
        if (COUNT_LED == 10)
        {
            off_all_invert();
        }
        else
        {
            off_all_invert_half();
        }
        flag_beg = n + 1;
        p_work.timer_game = p_work.period;
        Zumer();
    }
}

void chekBottonInvert (uint8_t bot)   /// �������� ������ �� ������
{
	if(chekInputBotton(GPIOA,bot))
		ifBottonOnInvert(bot);
}
