#include "beg.h"
extern uint16_t count_timer;
extern  param p_work;
extern uint8_t start_play,counter_number,random_numder[50],flag_beg,count_zumer,flag_zumer,count_mem,rand_number_duble_one[50],rand_number_duble_two[50];
//static uint8_t status=0,counter_beg=0,counter_cicl_beg=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Beg (void)                                 // ��� ������
{
		if((p_work.timer_game==p_work.period) )
		{
			p_work.timer_game=0;
			counter_number++;
			off_all();
		}
		if(counter_number<=p_work.number_cycles )
	    {
			count_timer=0;
			onLedBeg(random_numder[counter_number]);
			chekBotton(random_numder[counter_number]);
	    }
	    else
	    {
	    	off_all();
	    	start_play=2;
	    	flag_zumer=1;
        }
		if(flag_beg==0) p_work.timer_game++;


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onLedBeg (uint8_t on)
{
	switch(on)
	{
		case 0:
			on_one();
		    break;
		case 1:
			on_two();
		    break;
		case 2:
			on_three();
		    break;
		case 3:
			on_four();
		    break;
		case 4:
			on_five();
		    break;
		case 5:
			on_six();
		    break;
		case 6:
			on_seven();
		    break;
		case 7:
			on_eight();
		    break;
		case 8:
			on_nine();
		    break;
		case 9:
			on_ten();
		    break;
	}
}

void ifBottonOn (uint8_t n)    // ���� ��������� ������ ������
{
	  p_work.bal++;
	  off_all();
	  flag_beg=n+1;
	  p_work.timer_game=p_work.period;
	  Zumer();
}

void chekBotton (uint8_t bot)   /// �������� ������ �� ������
{
	if(chekInputBotton(GPIOA,bot))
		ifBottonOn(bot);
}
