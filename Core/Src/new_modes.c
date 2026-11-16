#include "new_modes.h"

/* Внешние глобальные переменные проекта */
extern param p_work;
extern uint8_t start_play, counter_number, flag_beg, flag_zumer, time_random, step, cicl_mem;
extern uint16_t count_timer;

/*
 * ==============================================================================
 * Массивы параметров генерации (размер 51 для циклов 1..50)
 * ==============================================================================
 */

/* BEG3: 1 мишень загорается, парную того же цвета нужно зажечь */
uint8_t rand_beg3_lit[51];
uint8_t rand_beg3_target[51];

/* BEG4: 2 мишени разного цвета горят, 2 парные нужно зажечь двумя руками */
uint8_t rand_beg4_lit1[51];
uint8_t rand_beg4_lit2[51];
uint8_t rand_beg4_t1[51];
uint8_t rand_beg4_t2[51];

/* INV2: 8 горят, 2 разного цвета не горят, зажечь обе двумя руками */
uint8_t rand_inv2_one[51];
uint8_t rand_inv2_two[51];

/* INV3: 9 горят кроме 1, погасить мишень того же цвета что и негорящая */
uint8_t rand_inv3_dark[51];
uint8_t rand_inv3_target[51];

/* INV4: 8 горят кроме 2 разного цвета, погасить 2 парные мишени двумя руками */
uint8_t rand_inv4_dark1[51];
uint8_t rand_inv4_dark2[51];
uint8_t rand_inv4_t1[51];
uint8_t rand_inv4_t2[51];

/* MEM2: последовательность пар мишеней для режима памяти */
uint8_t rand_mem2_one[51];
uint8_t rand_mem2_two[51];

/* DVA: 6 горят (4 разного цвета + 1 пара), погасить пару одного цвета */
uint8_t color_pair_one[51];
uint8_t color_pair_two[51];
uint16_t color_pair_lit_mask[51];

/* Флаги завершения удара в текущем цикле */
static uint8_t beg4_hit_done = 0;
static uint8_t inv2_hit_done = 0;
static uint8_t inv4_hit_done = 0;
static uint8_t color_hit_done = 0;

/*
 * ==============================================================================
 * Управление светодиодами мишеней (0..9)
 * ==============================================================================
 */
void setLedTarget(uint8_t target, uint8_t state)
{
    GPIO_PinState pin_state = (state != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    switch(target)
    {
        case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, pin_state); break;
        case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, pin_state); break;
        case 2: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, pin_state); break;
        case 3: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, pin_state); break;
        case 4: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  pin_state); break;
        case 5: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  pin_state); break;
        case 6: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  pin_state); break;
        case 7: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  pin_state); break;
        case 8: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,  pin_state); break;
        case 9: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,  pin_state); break;
        default: break;
    }
}

void onLed8On2Off(uint8_t off1, uint8_t off2)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        if (i == off1 || i == off2)
            setLedTarget(i, 0);
        else
            setLedTarget(i, 1);
    }
}

void onLedColorPair(uint8_t cycle)
{
    off_all();
    for (uint8_t i = 0; i < 10; i++)
    {
        if ((color_pair_lit_mask[cycle] & (1U << i)) != 0)
            setLedTarget(i, 1);
    }
}

/*
 * ==============================================================================
 * BEG3: Загорается 1 мишень. Надо зажигать мишень такого же цвета.
 * ==============================================================================
 */
void randomNumberBeg3(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t lit, last_lit = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            lit = rand() % 10;
        } while (lit == last_lit);

        last_lit = lit;
        rand_beg3_lit[i] = lit;
        rand_beg3_target[i] = getPartnerTarget(lit);
    }
}

void pollBeg3Buttons(void)
{
    if (start_play != 1 || flag_beg != 0 || p_work.mode != game_beg3)
        return;
    if (counter_number > p_work.number_cycles)
        return;

    uint8_t target = rand_beg3_target[counter_number];

    if (chekInputBotton(GPIOA, target))
    {
        /* Зажигаем парную мишень */
        setLedTarget(target, 1);
        p_work.bal++;
        Zumer();
        while (chekInputBotton(GPIOA, target));
        off_all();
        flag_beg = target + 1;
        p_work.timer_game = p_work.period;
    }
}

void beg3(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        off_all();

        if (counter_number > p_work.number_cycles)
        {
            off_all();
            start_play = 2;
            flag_zumer = 1;
            return;
        }
        else
        {
            setLedTarget(rand_beg3_lit[counter_number], 1);
        }
    }

    if (counter_number <= p_work.number_cycles)
    {
        count_timer = 0;
        if (flag_beg == 0)
        {
            setLedTarget(rand_beg3_lit[counter_number], 1);
        }
    }
    else
    {
        off_all();
        start_play = 2;
        flag_zumer = 1;
    }

    if (flag_beg == 0)
    {
        p_work.timer_game++;
    }
}

void checkReleaseBeg3(void)
{
    if (flag_beg > 0 && p_work.mode == game_beg3)
    {
        if (READ_BIT(GPIOA->IDR, (1U << (flag_beg - 1))))
        {
            flag_beg = 0;
        }
    }
}

/*
 * ==============================================================================
 * BEG4: Загораются по 2 мишени. Нужно зажигать двумя руками парные мишени.
 * ==============================================================================
 */
void randomNumberBeg4(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t l1, l2;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            l1 = rand() % 10;
            l2 = rand() % 10;
        } while (l2 == l1 || l2 == getPartnerTarget(l1));

        rand_beg4_lit1[i] = l1;
        rand_beg4_lit2[i] = l2;
        rand_beg4_t1[i] = getPartnerTarget(l1);
        rand_beg4_t2[i] = getPartnerTarget(l2);
    }
}

void pollBeg4Buttons(void)
{
    if (start_play != 1 || flag_beg != 0 || p_work.mode != game_beg4 || beg4_hit_done)
        return;
    if (counter_number > p_work.number_cycles)
        return;

    uint8_t t1 = rand_beg4_t1[counter_number];
    uint8_t t2 = rand_beg4_t2[counter_number];

    uint8_t hit1 = chekInputBotton(GPIOA, t1);
    uint8_t hit2 = chekInputBotton(GPIOA, t2);

    if (hit1 || hit2)
    {
        uint8_t second = hit1 ? t2 : t1;
        uint32_t t_start = HAL_GetTick();
        uint8_t second_pressed = (hit1 && hit2) ? 1 : 0;

        while (!second_pressed && (HAL_GetTick() - t_start) <= SIMULTANEOUS_PRESS_WINDOW_MS)
        {
            if (chekInputBotton(GPIOA, second))
            {
                second_pressed = 1;
                break;
            }
        }

        if (second_pressed)
        {
            setLedTarget(t1, 1);
            setLedTarget(t2, 1);
            p_work.bal++;
            Zumer();
            while (chekInputBotton(GPIOA, t1) || chekInputBotton(GPIOA, t2));
            off_all();
            beg4_hit_done = 1;
            p_work.timer_game = p_work.period;
            flag_beg = 10;
        }
    }
}

void beg4(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        beg4_hit_done = 0;
        off_all();

        if (counter_number > p_work.number_cycles)
        {
            off_all();
            start_play = 2;
            flag_zumer = 1;
            return;
        }
        else
        {
            setLedTarget(rand_beg4_lit1[counter_number], 1);
            setLedTarget(rand_beg4_lit2[counter_number], 1);
        }
    }

    if (counter_number <= p_work.number_cycles)
    {
        count_timer = 0;
        if (flag_beg == 0 && !beg4_hit_done)
        {
            setLedTarget(rand_beg4_lit1[counter_number], 1);
            setLedTarget(rand_beg4_lit2[counter_number], 1);
        }
    }
    else
    {
        off_all();
        start_play = 2;
        flag_zumer = 1;
    }

    if (flag_beg == 0)
    {
        p_work.timer_game++;
    }
}

void checkReleaseBeg4(void)
{
    if (flag_beg == 10 && p_work.mode == game_beg4)
    {
        uint8_t t1 = rand_beg4_t1[counter_number];
        uint8_t t2 = rand_beg4_t2[counter_number];
        if (READ_BIT(GPIOA->IDR, (1U << t1)) && READ_BIT(GPIOA->IDR, (1U << t2)))
        {
            flag_beg = 0;
        }
    }
}

/*
 * ==============================================================================
 * INV2: Горят 8 мишеней кроме двух разного цвета. Зажечь обе двумя руками.
 * ==============================================================================
 */
void randomNumberInvDouble(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t o1, o2;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            o1 = rand() % 10;
            o2 = rand() % 10;
        } while (o2 == o1 || o2 == getPartnerTarget(o1));

        rand_inv2_one[i] = o1;
        rand_inv2_two[i] = o2;
    }
}

void pollInvDoubleButtons(void)
{
    if (start_play != 1 || flag_beg != 0 || p_work.mode != game_inv_double || inv2_hit_done)
        return;
    if (counter_number > p_work.number_cycles)
        return;

    uint8_t t1 = rand_inv2_one[counter_number];
    uint8_t t2 = rand_inv2_two[counter_number];

    uint8_t hit1 = chekInputBotton(GPIOA, t1);
    uint8_t hit2 = chekInputBotton(GPIOA, t2);

    if (hit1 || hit2)
    {
        uint8_t second = hit1 ? t2 : t1;
        uint32_t t_start = HAL_GetTick();
        uint8_t second_pressed = (hit1 && hit2) ? 1 : 0;

        while (!second_pressed && (HAL_GetTick() - t_start) <= SIMULTANEOUS_PRESS_WINDOW_MS)
        {
            if (chekInputBotton(GPIOA, second))
            {
                second_pressed = 1;
                break;
            }
        }

        if (second_pressed)
        {
            setLedTarget(t1, 1);
            setLedTarget(t2, 1);
            p_work.bal++;
            Zumer();
            while (chekInputBotton(GPIOA, t1) || chekInputBotton(GPIOA, t2));
            off_all();
            inv2_hit_done = 1;
            p_work.timer_game = p_work.period;
            flag_beg = 10;
        }
    }
}

void inv_double(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        inv2_hit_done = 0;
        off_all();

        if (counter_number > p_work.number_cycles)
        {
            off_all();
            start_play = 2;
            flag_zumer = 1;
            return;
        }
        else
        {
            onLed8On2Off(rand_inv2_one[counter_number], rand_inv2_two[counter_number]);
        }
    }

    if (counter_number <= p_work.number_cycles)
    {
        count_timer = 0;
        if (flag_beg == 0 && !inv2_hit_done)
        {
            onLed8On2Off(rand_inv2_one[counter_number], rand_inv2_two[counter_number]);
        }
    }
    else
    {
        off_all();
        start_play = 2;
        flag_zumer = 1;
    }

    if (flag_beg == 0)
    {
        p_work.timer_game++;
    }
}

void checkReleaseInvDouble(void)
{
    if (flag_beg == 10 && p_work.mode == game_inv_double)
    {
        uint8_t t1 = rand_inv2_one[counter_number];
        uint8_t t2 = rand_inv2_two[counter_number];
        if (READ_BIT(GPIOA->IDR, (1U << t1)) && READ_BIT(GPIOA->IDR, (1U << t2)))
        {
            flag_beg = 0;
        }
    }
}

/*
 * ==============================================================================
 * INV3: Горят 9 мишеней кроме одной. Погасить мишень того же цвета, что негорящая.
 * ==============================================================================
 */
void randomNumberInv3(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t dark, last_dark = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            dark = rand() % 10;
        } while (dark == last_dark);

        last_dark = dark;
        rand_inv3_dark[i] = dark;
        rand_inv3_target[i] = getPartnerTarget(dark);
    }
}

void pollInv3Buttons(void)
{
    if (start_play != 1 || flag_beg != 0 || p_work.mode != game_inv3)
        return;
    if (counter_number > p_work.number_cycles)
        return;

    uint8_t target = rand_inv3_target[counter_number];

    if (chekInputBotton(GPIOA, target))
    {
        setLedTarget(target, 0);
        p_work.bal++;
        Zumer();
        while (chekInputBotton(GPIOA, target));
        off_all();
        flag_beg = target + 1;
        p_work.timer_game = p_work.period;
    }
}

void inv3(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        off_all();

        if (counter_number > p_work.number_cycles)
        {
            off_all();
            start_play = 2;
            flag_zumer = 1;
            return;
        }
        else
        {
            for (uint8_t i = 0; i < 10; i++)
            {
                setLedTarget(i, (i != rand_inv3_dark[counter_number]) ? 1 : 0);
            }
        }
    }

    if (counter_number <= p_work.number_cycles)
    {
        count_timer = 0;
        if (flag_beg == 0)
        {
            for (uint8_t i = 0; i < 10; i++)
            {
                setLedTarget(i, (i != rand_inv3_dark[counter_number]) ? 1 : 0);
            }
        }
    }
    else
    {
        off_all();
        start_play = 2;
        flag_zumer = 1;
    }

    if (flag_beg == 0)
    {
        p_work.timer_game++;
    }
}

void checkReleaseInv3(void)
{
    if (flag_beg > 0 && p_work.mode == game_inv3)
    {
        if (READ_BIT(GPIOA->IDR, (1U << (flag_beg - 1))))
        {
            flag_beg = 0;
        }
    }
}

/*
 * ==============================================================================
 * INV4: Горят 8 мишеней кроме двух разного цвета. Погасить 2 парные мишени двумя руками.
 * ==============================================================================
 */
void randomNumberInv4(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t d1, d2;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            d1 = rand() % 10;
            d2 = rand() % 10;
        } while (d2 == d1 || d2 == getPartnerTarget(d1));

        rand_inv4_dark1[i] = d1;
        rand_inv4_dark2[i] = d2;
        rand_inv4_t1[i] = getPartnerTarget(d1);
        rand_inv4_t2[i] = getPartnerTarget(d2);
    }
}

void pollInv4Buttons(void)
{
    if (start_play != 1 || flag_beg != 0 || p_work.mode != game_inv4 || inv4_hit_done)
        return;
    if (counter_number > p_work.number_cycles)
        return;

    uint8_t t1 = rand_inv4_t1[counter_number];
    uint8_t t2 = rand_inv4_t2[counter_number];

    uint8_t hit1 = chekInputBotton(GPIOA, t1);
    uint8_t hit2 = chekInputBotton(GPIOA, t2);

    if (hit1 || hit2)
    {
        uint8_t second = hit1 ? t2 : t1;
        uint32_t t_start = HAL_GetTick();
        uint8_t second_pressed = (hit1 && hit2) ? 1 : 0;

        while (!second_pressed && (HAL_GetTick() - t_start) <= SIMULTANEOUS_PRESS_WINDOW_MS)
        {
            if (chekInputBotton(GPIOA, second))
            {
                second_pressed = 1;
                break;
            }
        }

        if (second_pressed)
        {
            setLedTarget(t1, 0);
            setLedTarget(t2, 0);
            p_work.bal++;
            Zumer();
            while (chekInputBotton(GPIOA, t1) || chekInputBotton(GPIOA, t2));
            off_all();
            inv4_hit_done = 1;
            p_work.timer_game = p_work.period;
            flag_beg = 10;
        }
    }
}

void inv4(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        inv4_hit_done = 0;
        off_all();

        if (counter_number > p_work.number_cycles)
        {
            off_all();
            start_play = 2;
            flag_zumer = 1;
            return;
        }
        else
        {
            onLed8On2Off(rand_inv4_dark1[counter_number], rand_inv4_dark2[counter_number]);
        }
    }

    if (counter_number <= p_work.number_cycles)
    {
        count_timer = 0;
        if (flag_beg == 0 && !inv4_hit_done)
        {
            onLed8On2Off(rand_inv4_dark1[counter_number], rand_inv4_dark2[counter_number]);
        }
    }
    else
    {
        off_all();
        start_play = 2;
        flag_zumer = 1;
    }

    if (flag_beg == 0)
    {
        p_work.timer_game++;
    }
}

void checkReleaseInv4(void)
{
    if (flag_beg == 10 && p_work.mode == game_inv4)
    {
        uint8_t t1 = rand_inv4_t1[counter_number];
        uint8_t t2 = rand_inv4_t2[counter_number];
        if (READ_BIT(GPIOA->IDR, (1U << t1)) && READ_BIT(GPIOA->IDR, (1U << t2)))
        {
            flag_beg = 0;
        }
    }
}

/*
 * ==============================================================================
 * MEM2: Память пар мишеней. Зажигаются по 2 мишени. Повторить двумя руками.
 * ==============================================================================
 */
void randomNumberMemDouble(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t p1, p2;
    uint8_t last_p1 = 255, last_p2 = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            p1 = rand() % 10;
            p2 = rand() % 10;
            while (p2 == p1)
            {
                p2 = rand() % 10;
            }
        } while ((p1 == last_p1 && p2 == last_p2) || (p1 == last_p2 && p2 == last_p1));

        last_p1 = p1;
        last_p2 = p2;
        rand_mem2_one[i] = p1;
        rand_mem2_two[i] = p2;
    }
}

void mem_double_timer(void)
{
    if (step == 1)
    {
        if (p_work.timer_game >= p_work.period)
        {
            p_work.timer_game = 0;
            counter_number++;
            off_all();
        }

        if (counter_number <= p_work.number_cycles)
        {
            count_timer = 0;
            setLedTarget(rand_mem2_one[counter_number], 1);
            setLedTarget(rand_mem2_two[counter_number], 1);
        }
        else
        {
            off_all();
            step = 2;
            cicl_mem = 1;
        }

        if (flag_beg == 0)
        {
            p_work.timer_game++;
        }
    }
}

void checkInputMemDouble(void)
{
    if (step != 2 || p_work.mode != game_mem_double)
    {
        return;
    }

    uint8_t exp1 = rand_mem2_one[cicl_mem];
    uint8_t exp2 = rand_mem2_two[cicl_mem];

    for (uint8_t i = 0; i < 10; i++)
    {
        if (chekInputMem(GPIOA, i))
        {
            if (i != exp1 && i != exp2)
            {
                step = 5;
                ZumerError();
                return;
            }

            uint8_t second = (i == exp1) ? exp2 : exp1;
            uint32_t t_start = HAL_GetTick();
            uint8_t second_pressed = 0;

            while ((HAL_GetTick() - t_start) <= SIMULTANEOUS_PRESS_WINDOW_MS)
            {
                if (chekInputMem(GPIOA, second))
                {
                    second_pressed = 1;
                    break;
                }
                for (uint8_t j = 0; j < 10; j++)
                {
                    if (j != exp1 && j != exp2 && chekInputMem(GPIOA, j))
                    {
                        step = 5;
                        ZumerError();
                        return;
                    }
                }
            }

            if (second_pressed)
            {
                setLedTarget(exp1, 1);
                setLedTarget(exp2, 1);
                p_work.bal++;
                cicl_mem++;
                Zumer();

                while (chekInputMem(GPIOA, exp1) || chekInputMem(GPIOA, exp2));
                off_all();

                if (p_work.bal == p_work.number_cycles)
                {
                    lcd_goto(14, 1);
                    lcd_number(p_work.bal);
                    step = 5;
                    ZumerOk();
                }
            }
            return;
        }
    }
}

/*
 * ==============================================================================
 * DVA: 6 горят (4 разного цвета + 1 пара одного цвета). Погасить пару одного цвета.
 * ==============================================================================
 */
void randomNumberColorPair(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t last_dup = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        uint8_t dup_color;
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            dup_color = rand() % 5;
        } while (dup_color == last_dup);
        last_dup = dup_color;

        color_pair_one[i] = dup_color;
        color_pair_two[i] = dup_color + 5;

        uint16_t mask = (1U << dup_color) | (1U << (dup_color + 5));

        for (uint8_t c = 0; c < 5; c++)
        {
            if (c != dup_color)
            {
                uint8_t target = (rand() % 2 == 0) ? c : (c + 5);
                mask |= (1U << target);
            }
        }
        color_pair_lit_mask[i] = mask;
    }
}

void pollColorPairButtons(void)
{
    if (start_play != 1 || flag_beg != 0 || p_work.mode != game_color_pair || color_hit_done)
        return;
    if (counter_number > p_work.number_cycles)
        return;

    uint8_t t1 = color_pair_one[counter_number];
    uint8_t t2 = color_pair_two[counter_number];

    uint8_t hit1 = chekInputBotton(GPIOA, t1);
    uint8_t hit2 = chekInputBotton(GPIOA, t2);

    if (hit1 || hit2)
    {
        uint8_t second = hit1 ? t2 : t1;
        uint32_t t_start = HAL_GetTick();
        uint8_t second_pressed = (hit1 && hit2) ? 1 : 0;

        while (!second_pressed && (HAL_GetTick() - t_start) <= SIMULTANEOUS_PRESS_WINDOW_MS)
        {
            if (chekInputBotton(GPIOA, second))
            {
                second_pressed = 1;
                break;
            }
        }

        if (second_pressed)
        {
            off_all();
            p_work.bal++;
            Zumer();
            while (chekInputBotton(GPIOA, t1) || chekInputBotton(GPIOA, t2));
            color_hit_done = 1;
            p_work.timer_game = p_work.period;
            flag_beg = 10;
        }
    }
}

void color_pair(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        color_hit_done = 0;
        off_all();

        if (counter_number > p_work.number_cycles)
        {
            off_all();
            start_play = 2;
            flag_zumer = 1;
            return;
        }
        else
        {
            onLedColorPair(counter_number);
        }
    }

    if (counter_number <= p_work.number_cycles)
    {
        count_timer = 0;
        if (flag_beg == 0 && !color_hit_done)
        {
            onLedColorPair(counter_number);
        }
    }
    else
    {
        off_all();
        start_play = 2;
        flag_zumer = 1;
    }

    if (flag_beg == 0)
    {
        p_work.timer_game++;
    }
}

void checkReleaseColorPair(void)
{
    if (flag_beg == 10 && p_work.mode == game_color_pair)
    {
        uint8_t t1 = color_pair_one[counter_number];
        uint8_t t2 = color_pair_two[counter_number];
        if (READ_BIT(GPIOA->IDR, (1U << t1)) && READ_BIT(GPIOA->IDR, (1U << t2)))
        {
            flag_beg = 0;
        }
    }
}
