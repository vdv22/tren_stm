#include "SetParametr.h"
extern  param p_work;
extern const uint8_t temp_numder[6];
extern uint8_t flag_beg,flag_zumer;
extern uint16_t count_timer;
extern uint8_t position;
extern uint8_t start_play;

static void display_mode_name(uint8_t mode)
{
    lcd_goto(12, 0);
    switch (mode)
    {
        case game_beg:        lcd_string("BEG "); break;
        case game_duble:      lcd_string("BEG2"); break;
        case game_beg3:       lcd_string("BEG3"); break;
        case game_beg4:       lcd_string("BEG4"); break;
        case game_invers:     lcd_string("INV "); break;
        case game_inv_double: lcd_string("INV2"); break;
        case game_inv3:       lcd_string("INV3"); break;
        case game_inv4:       lcd_string("INV4"); break;
        case mem_game:        lcd_string("MEM "); break;
        case game_mem_double: lcd_string("MEM2"); break;
        case game_color_pair: lcd_string("DVA "); break;
        case test:            lcd_string("TST "); break;
        default:              lcd_string("BEG "); break;
    }
}

void SetParametrPlay (void)          //set speed and quantity
{
    while (bt_ok()) { HAL_Delay(10); }
    HAL_Delay(300);

    int i = position;
    if (i >= 6) i = 3; /* default: 10 cycles */

    uint8_t count_ = 0;

    /* --- ШАГ 1: КОЛИЧЕСТВО ЦИКЛОВ (KOLV) --- */
    while (count_ < 6)
    {
        count_++;
        lcd_goto(5, 0);
        lcd_string("  ");
        HAL_Delay(200);
        lcd_goto(5, 0);
        lcd_number(temp_numder[i]);
        if (temp_numder[i] < 10) lcd_string(" ");
        HAL_Delay(200);

        if (bt_plus() && i < 5)
        {
            i++;
            count_ = 0;
            lcd_goto(5, 0);
            lcd_number(temp_numder[i]);
            if (temp_numder[i] < 10) lcd_string(" ");
            HAL_Delay(250);
        }
        if (bt_minus() && i > 0)
        {
            i--;
            count_ = 0;
            lcd_goto(5, 0);
            lcd_number(temp_numder[i]);
            if (temp_numder[i] < 10) lcd_string(" ");
            HAL_Delay(250);
        }
    }

    p_work.number_cycles = temp_numder[i];
    position = (uint8_t)i;

    lcd_goto(5, 0);
    lcd_number(temp_numder[i]);
    if (temp_numder[i] < 10) lcd_string(" ");

    /* --- ШАГ 2: ТЕМП (TEMP) --- */
    count_ = 0;
    i = p_work.period;
    if (i < 2 || i > 20) i = 15;

    while (count_ < 6)
    {
        count_++;
        lcd_goto(5, 1);
        lcd_string("   ");
        HAL_Delay(200);

        lcd_goto(5, 1);
        if (i < 10)
        {
            lcd_string("0,");
            lcd_number(i);
        }
        else if (i >= 10 && i < 20)
        {
            lcd_string("1,");
            lcd_number(i % 10);
        }
        else
        {
            lcd_string("2,0");
        }
        HAL_Delay(200);

        if (bt_plus())
        {
            if (i < 10) i++;
            else if (i >= 10 && i < 20) i += 5;
            if (i > 20) i = 20;
            count_ = 0;
            lcd_goto(5, 1);
            if (i < 10) { lcd_string("0,"); lcd_number(i); }
            else if (i >= 10 && i < 20) { lcd_string("1,"); lcd_number(i % 10); }
            else { lcd_string("2,0"); }
            HAL_Delay(250);
        }

        if (bt_minus())
        {
            if (i > 10) i -= 5;
            else if (i > 2 && i <= 10) i--;
            if (i < 2) i = 2;
            count_ = 0;
            lcd_goto(5, 1);
            if (i < 10) { lcd_string("0,"); lcd_number(i); }
            else if (i >= 10 && i < 20) { lcd_string("1,"); lcd_number(i % 10); }
            else { lcd_string("2,0"); }
            HAL_Delay(250);
        }
    }

    p_work.period = (uint8_t)i;
    int_mem_write();
    lcd_clear();
    indication();
    HAL_Delay(200);

    if (p_work.mode != test)
    {
        on_one();
        start_play = 0;
    }
    else
    {
        off_all();
        start_play = 0;
    }
}

void SetMode(void)                              //Set mode GAME
{
    /* Сразу выводим экран настройки, чтобы пользователь увидел меню прямо во время зажатия кнопки! */
    lcd_goto(0, 0);
    lcd_string("HACTP. MODE-");
    lcd_goto(0, 1);
    lcd_string("                ");
    display_mode_name(p_work.mode);

    /* Теперь ждём, когда пользователь отпустит кнопку RESET */
    while (bt_reset()) { HAL_Delay(10); }
    HAL_Delay(200);

    while (1)
    {
        /* Exit and confirm on OK (Red) or RESET (White) */
        if (bt_ok() || bt_reset())
        {
            while (bt_ok() || bt_reset()) { HAL_Delay(10); }
            HAL_Delay(150);
            break;
        }

        if (bt_plus())
        {
            if (p_work.mode < 11)
            {
                p_work.mode++;
            }
            else
            {
                p_work.mode = 0; /* wrap around */
            }
            display_mode_name(p_work.mode);
            uint32_t hold_start = HAL_GetTick();
            while (bt_plus())
            {
                if (HAL_GetTick() - hold_start > 350) break;
                HAL_Delay(10);
            }
        }

        if (bt_minus())
        {
            if (p_work.mode > 0)
            {
                p_work.mode--;
            }
            else
            {
                p_work.mode = 11; /* wrap around */
            }
            display_mode_name(p_work.mode);
            uint32_t hold_start = HAL_GetTick();
            while (bt_minus())
            {
                if (HAL_GetTick() - hold_start > 350) break;
                HAL_Delay(10);
            }
        }

        HAL_Delay(10);
    }

    int_mem_write();
    lcd_clear();
    indication();
    HAL_Delay(200);

    if (p_work.mode != test)
    {
        on_one();
        start_play = 0;
    }
    else
    {
        off_all();
        start_play = 0;
    }
}

void  ZumerOk(void)
{
    for(int i=0;i<3;i++)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_Delay(100);
    }
}

void ZumerError (void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void Zumer (void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    for (volatile uint32_t count = 0; count < 500000; count++)
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
