#include "doubleGame.h"
extern  param p_work;
extern const uint8_t temp_numder[7];
extern uint8_t flag_beg,flag_zumer,time_random,counter_number,start_play;
extern uint16_t count_timer;
extern uint8_t rand_number_duble_one[50],rand_number_duble_two[50]; // Массив со случайными числами.

void randomNumberDuble (void)         // готовим массив из рандомов
{
	uint8_t out=0,out_old;
	for(uint8_t i=1;i<=p_work.number_cycles;i++)
	{
		 if(time_random>50) {time_random=0;} else {time_random++;}
		 time_random++;
		out=(rand()%5);
		srand(time_random);
	    while(out_old==out)
	    {
	    	out=(rand()%5);
	    }
	    out_old=out;
	    rand_number_duble_one[i]=out;
	}
	for(uint8_t i=1;i<=p_work.number_cycles;i++)
	{
		while(out_old==out)
		{
			out=(rand()%5);
		}
		out_old=out;
		rand_number_duble_two[i]=out;
	}
}

int out (uint8_t one,uint8_t two)
{
	uint8_t chek=0;
    switch(one)
	{
    	case 1:
    		if(in_one()!=0)
    			chek++;
    		break;
    	case 2:
    		if(in_two()!=0)
    			chek++;
    		break;
    	case 3:
    		if(in_three()!=0)
    			chek++;
  		   break;
    	case 4:
    		if(in_four()!=0)
    			chek++;
  		   break;
    	case 5:
    		if(in_five()!=0)
    			chek++;
  		   break;
	}

    switch(two)
	{
		case 1:
			if(in_six()!=0)
				chek++;
			break;
		case 2:
			if(in_seven()!=0)
				chek++;
			break;
		case 3:
			if(in_eight()!=0)
				chek++;
			break;
		case 4:
			if(in_nine()!=0)
				chek++;
			break;
		case 5:
			if(in_ten()!=0)
				chek++;
			break;
	}
return chek;
}

void duble (void)
{
	if((p_work.timer_game==p_work.period) )
	{
		p_work.duble_two_trye=0;
		p_work.duble_one_trye=0;
		p_work.duble_one_flag=0;
		p_work.duble_two_flag=0;
		p_work.timer_game=0;
		counter_number++;
		off_all();
	}
	if( counter_number<=p_work.number_cycles )
	{
		count_timer=0;
		onLedDouble(rand_number_duble_one[counter_number],rand_number_duble_two[counter_number]);

	switch(rand_number_duble_one[counter_number])
	{
		case 0:
			if(!(in_one()))
			{
				p_work.duble_one_trye=1;
				p_work.duble_one_flag=1;
			}
				break;
		case 1:
			if(!(in_two()))
			{
				p_work.duble_one_trye=1;
				p_work.duble_one_flag=2;
			}
				break;
		case 2:
			if(!(in_three()))
			{
				p_work.duble_one_trye=1;
				p_work.duble_one_flag=3;
			}
				break;
		case 3:
			if(!(in_four()))
			{
				p_work.duble_one_trye=1;
				p_work.duble_one_flag=4;
			}
				break;
		case 4:
			if(!(in_five()))
			{
				p_work.duble_one_trye=1;
				p_work.duble_one_flag=5;
			}
				break;
	}

	switch(rand_number_duble_two[counter_number])     //sekond half
	{
		case 0:
			if(!(in_six()))
			{
				p_work.duble_two_trye=1;
				p_work.duble_two_flag=1;
			}
			break;
		case 1:
			if(!(in_seven()))
    		{
				p_work.duble_two_trye=1;
				p_work.duble_two_flag=2;
    		}
			break;
		case 2:
			if(!(in_eight()))
    		{
				p_work.duble_two_trye=1;
				p_work.duble_two_flag=3;
    		}
			break;
		case 3:
			if(!(in_nine()))
    		{
				p_work.duble_two_trye=1;
				p_work.duble_two_flag=4;
    		}
			break;
		case 4:
			if(!(in_ten()))
    		{
				p_work.duble_two_trye=1;
				p_work.duble_two_flag=5;
            }
			break;
	}

	}
	else
	{
		off_all();
		start_play=2;
		flag_zumer=1;
	}
	if(p_work.duble_two_trye==1 &&  p_work.duble_one_trye==1 && flag_beg!=10)
	{
		p_work.bal++;
		off_all();
		Zumer();
		flag_beg=10;
		p_work.timer_game=p_work.period;
	}
	if(flag_beg==0) p_work.timer_game++;
}

void onLedDouble (uint8_t one,uint8_t two)
{
	switch(one)
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
	}
	switch(two)
	{
		case 0:
			on_six();
		    break;
		case 1:
			on_seven();
		    break;
		case 2:
			on_eight();
		    break;
		case 3:
			on_nine();
		    break;
		case 4:
			on_ten();
		    break;
	}
}
