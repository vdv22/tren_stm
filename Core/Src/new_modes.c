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

/* Независимые триггеры фиксации касания для двух рук */
static uint8_t beg4_latch1 = 0, beg4_latch2 = 0;
static uint8_t inv2_latch1 = 0, inv2_latch2 = 0;
static uint8_t inv4_latch1 = 0, inv4_latch2 = 0;
static uint8_t dva_latch1 = 0,  dva_latch2 = 0;

/* Защита от двойного начисления баллов в одном цикле */
static uint8_t beg3_scored_cycle = 255;
static uint8_t beg4_scored_cycle = 255;
static uint8_t inv2_scored_cycle = 255;
static uint8_t inv3_scored_cycle = 255;
static uint8_t inv4_scored_cycle = 255;
static uint8_t dva_scored_cycle  = 255;

/* Запоминаем пины для надёжной проверки отпускания */
static uint8_t beg3_rel1 = 255, beg3_rel2 = 255;
static uint8_t inv3_rel1 = 255, inv3_rel2 = 255;

void reset_all_mode_latches(void)
{
    beg3_scored_cycle = 255;
    beg3_rel1 = 255; beg3_rel2 = 255;

    beg4_scored_cycle = 255;
    beg4_latch1 = 0;
    beg4_latch2 = 0;
    beg4_hit_done = 0;

    inv2_scored_cycle = 255;
    inv2_latch1 = 0;
    inv2_latch2 = 0;
    inv2_hit_done = 0;

    inv3_scored_cycle = 255;
    inv3_rel1 = 255; inv3_rel2 = 255;

    inv4_scored_cycle = 255;
    inv4_latch1 = 0;
    inv4_latch2 = 0;
    inv4_hit_done = 0;

    dva_scored_cycle = 255;
    dva_latch1 = 0;
    dva_latch2 = 0;
    color_hit_done = 0;
}

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
    off_all();
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
 * BEG3: Загорается 1 мишень. Надо зажечь мишень такого же цвета.
 * ==============================================================================
 */
void randomNumberBeg3(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t lit;
    uint8_t last_c = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        uint8_t c;
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            lit = rand() % 10;
            c = lit % 5;
        } while (c == last_c); /* Не повторять цвет подряд */

        last_c = c;
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

    uint8_t t_lit = rand_beg3_lit[counter_number];
    uint8_t t_partner = rand_beg3_target[counter_number];

    /* Принимаем касание ЛЮБОЙ мишени данного цвета (горящую или парную) */
    if ((chekInputBotton(GPIOA, t_partner) || chekInputBotton(GPIOA, t_lit)) && beg3_scored_cycle != counter_number)
    {
        beg3_scored_cycle = counter_number;
        setLedTarget(t_partner, 1); /* Зажигаем обе для наглядности */
        setLedTarget(t_lit, 1);
        if (p_work.bal < p_work.number_cycles)
        {
            p_work.bal++;
        }
        beg3_rel1 = t_lit;
        beg3_rel2 = t_partner;
        flag_beg = 10;
        p_work.timer_game = p_work.period;
        Zumer();
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
            off_all();
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
    if (flag_beg == 10 && p_work.mode == game_beg3)
    {
        uint8_t ok = 1;
        if (beg3_rel1 < 10 && (READ_BIT(GPIOA->IDR, (1U << beg3_rel1)) == RESET)) ok = 0;
        if (beg3_rel2 < 10 && (READ_BIT(GPIOA->IDR, (1U << beg3_rel2)) == RESET)) ok = 0;

        if (ok)
        {
            off_all();
            flag_beg = 0;
        }
    }
}

/*
 * ==============================================================================
 * BEG4: Загораются 2 мишени разного цвета. Зажечь двумя руками парные мишени.
 * ==============================================================================
 */
void randomNumberBeg4(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t l1 = 255, l2 = 255;
    uint8_t c1, c2;
    uint8_t last_c1 = 255, last_c2 = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            l1 = rand() % 10;
            l2 = rand() % 10;
            c1 = l1 % 5;
            c2 = l2 % 5;
        } while (c1 == c2 ||
                 c1 == last_c1 || c1 == last_c2 ||
                 c2 == last_c1 || c2 == last_c2); /* Разные цвета и БЕЗ повтора предыдущих мишеней */

        last_c1 = c1;
        last_c2 = c2;
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

    uint8_t l1 = rand_beg4_lit1[counter_number];
    uint8_t l2 = rand_beg4_lit2[counter_number];
    uint8_t t1 = rand_beg4_t1[counter_number];
    uint8_t t2 = rand_beg4_t2[counter_number];

    /* Касание цвета 1 (горящая l1 или парная t1) */
    if (chekInputBotton(GPIOA, l1) || chekInputBotton(GPIOA, t1))
    {
        beg4_latch1 = 1;
        setLedTarget(l1, 1);
        setLedTarget(t1, 1);
    }
    /* Касание цвета 2 (горящая l2 или парная t2) */
    if (chekInputBotton(GPIOA, l2) || chekInputBotton(GPIOA, t2))
    {
        beg4_latch2 = 1;
        setLedTarget(l2, 1);
        setLedTarget(t2, 1);
    }

    if (beg4_latch1 && beg4_latch2 && beg4_scored_cycle != counter_number)
    {
        beg4_scored_cycle = counter_number;
        if (p_work.bal < p_work.number_cycles)
        {
            p_work.bal++;
        }
        beg4_hit_done = 1;
        flag_beg = 10;
        p_work.timer_game = p_work.period;
        Zumer();
    }
}

void beg4(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        beg4_hit_done = 0;
        beg4_latch1 = 0;
        beg4_latch2 = 0;
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
            off_all();
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
            if (beg4_latch1) setLedTarget(rand_beg4_t1[counter_number], 1);
            if (beg4_latch2) setLedTarget(rand_beg4_t2[counter_number], 1);
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
        uint8_t ok = 1;
        uint8_t l1 = rand_beg4_lit1[counter_number];
        uint8_t l2 = rand_beg4_lit2[counter_number];
        uint8_t t1 = rand_beg4_t1[counter_number];
        uint8_t t2 = rand_beg4_t2[counter_number];
        if (READ_BIT(GPIOA->IDR, (1U << l1)) == RESET ||
            READ_BIT(GPIOA->IDR, (1U << l2)) == RESET ||
            READ_BIT(GPIOA->IDR, (1U << t1)) == RESET ||
            READ_BIT(GPIOA->IDR, (1U << t2)) == RESET)
        {
            ok = 0;
        }

        if (ok)
        {
            off_all();
            beg4_latch1 = 0;
            beg4_latch2 = 0;
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
    uint8_t o1 = 255, o2 = 255;
    uint8_t c1, c2;
    uint8_t last_c1 = 255, last_c2 = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            o1 = rand() % 10;
            o2 = rand() % 10;
            c1 = o1 % 5;
            c2 = o2 % 5;
        } while (c1 == c2 ||
                 c1 == last_c1 || c1 == last_c2 ||
                 c2 == last_c1 || c2 == last_c2); /* Разные цвета и БЕЗ повтора предыдущих мишеней */

        last_c1 = c1;
        last_c2 = c2;
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

    if (chekInputBotton(GPIOA, t1))
    {
        inv2_latch1 = 1;
        setLedTarget(t1, 1); /* Зажигаем негорящую мишень при касании */
    }
    if (chekInputBotton(GPIOA, t2))
    {
        inv2_latch2 = 1;
        setLedTarget(t2, 1); /* Зажигаем негорящую мишень при касании */
    }

    if (inv2_latch1 && inv2_latch2 && inv2_scored_cycle != counter_number)
    {
        inv2_scored_cycle = counter_number;
        if (p_work.bal < p_work.number_cycles)
        {
            p_work.bal++;
        }
        inv2_hit_done = 1;
        flag_beg = 10;
        p_work.timer_game = p_work.period;
        Zumer();
    }
}

void inv_double(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        inv2_hit_done = 0;
        inv2_latch1 = 0;
        inv2_latch2 = 0;
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
            if (inv2_latch1) setLedTarget(rand_inv2_one[counter_number], 1);
            if (inv2_latch2) setLedTarget(rand_inv2_two[counter_number], 1);
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
            HAL_Delay(15);
            if (READ_BIT(GPIOA->IDR, (1U << t1)) && READ_BIT(GPIOA->IDR, (1U << t2)))
            {
                off_all();
                inv2_latch1 = 0;
                inv2_latch2 = 0;
                flag_beg = 0;
            }
        }
    }
}

/*
 * ==============================================================================
 * INV3: Горят 9 мишеней кроме одной. Погасить мишень того же цвета что и негорящая.
 * ==============================================================================
 */
void randomNumberInv3(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t dark;
    uint8_t last_c = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        uint8_t c;
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            dark = rand() % 10;
            c = dark % 5;
        } while (c == last_c); /* Не повторять цвет подряд */

        last_c = c;
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

    uint8_t dark = rand_inv3_dark[counter_number];
    uint8_t target = rand_inv3_target[counter_number];

    /* Принимаем любую мишень данного цвета (негорящую dark или горящую target) */
    if ((chekInputBotton(GPIOA, target) || chekInputBotton(GPIOA, dark)) && inv3_scored_cycle != counter_number)
    {
        inv3_scored_cycle = counter_number;
        setLedTarget(target, 0); /* Гасим обе мишени цвета */
        setLedTarget(dark, 0);
        if (p_work.bal < p_work.number_cycles)
        {
            p_work.bal++;
        }
        inv3_rel1 = dark;
        inv3_rel2 = target;
        flag_beg = 10;
        p_work.timer_game = p_work.period;
        Zumer();
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
            off_all();
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
    if (flag_beg == 10 && p_work.mode == game_inv3)
    {
        uint8_t ok = 1;
        if (inv3_rel1 < 10 && (READ_BIT(GPIOA->IDR, (1U << inv3_rel1)) == RESET)) ok = 0;
        if (inv3_rel2 < 10 && (READ_BIT(GPIOA->IDR, (1U << inv3_rel2)) == RESET)) ok = 0;

        if (ok)
        {
            off_all();
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
    uint8_t d1 = 255, d2 = 255;
    uint8_t c1, c2;
    uint8_t last_c1 = 255, last_c2 = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        do {
            if (time_random > 50) { time_random = 0; } else { time_random++; }
            d1 = rand() % 10;
            d2 = rand() % 10;
            c1 = d1 % 5;
            c2 = d2 % 5;
        } while (c1 == c2 ||
                 c1 == last_c1 || c1 == last_c2 ||
                 c2 == last_c1 || c2 == last_c2); /* Разные цвета и БЕЗ повтора предыдущих мишеней */

        last_c1 = c1;
        last_c2 = c2;
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

    uint8_t d1 = rand_inv4_dark1[counter_number];
    uint8_t d2 = rand_inv4_dark2[counter_number];
    uint8_t t1 = rand_inv4_t1[counter_number];
    uint8_t t2 = rand_inv4_t2[counter_number];

    /* Касание цвета 1 (негорящая d1 или горящая t1) */
    if (chekInputBotton(GPIOA, d1) || chekInputBotton(GPIOA, t1))
    {
        inv4_latch1 = 1;
        setLedTarget(d1, 0);
        setLedTarget(t1, 0);
    }
    /* Касание цвета 2 (негорящая d2 или горящая t2) */
    if (chekInputBotton(GPIOA, d2) || chekInputBotton(GPIOA, t2))
    {
        inv4_latch2 = 1;
        setLedTarget(d2, 0);
        setLedTarget(t2, 0);
    }

    if (inv4_latch1 && inv4_latch2 && inv4_scored_cycle != counter_number)
    {
        inv4_scored_cycle = counter_number;
        if (p_work.bal < p_work.number_cycles)
        {
            p_work.bal++;
        }
        inv4_hit_done = 1;
        flag_beg = 10;
        p_work.timer_game = p_work.period;
        Zumer();
    }
}

void inv4(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        inv4_hit_done = 0;
        inv4_latch1 = 0;
        inv4_latch2 = 0;
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
            if (inv4_latch1) setLedTarget(rand_inv4_t1[counter_number], 0);
            if (inv4_latch2) setLedTarget(rand_inv4_t2[counter_number], 0);
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
        uint8_t ok = 1;
        uint8_t d1 = rand_inv4_dark1[counter_number];
        uint8_t d2 = rand_inv4_dark2[counter_number];
        uint8_t t1 = rand_inv4_t1[counter_number];
        uint8_t t2 = rand_inv4_t2[counter_number];
        if (READ_BIT(GPIOA->IDR, (1U << d1)) == RESET ||
            READ_BIT(GPIOA->IDR, (1U << d2)) == RESET ||
            READ_BIT(GPIOA->IDR, (1U << t1)) == RESET ||
            READ_BIT(GPIOA->IDR, (1U << t2)) == RESET)
        {
            ok = 0;
        }

        if (ok)
        {
            off_all();
            inv4_latch1 = 0;
            inv4_latch2 = 0;
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
        } while (p1 == p2 ||
                 p1 == last_p1 || p1 == last_p2 ||
                 p2 == last_p1 || p2 == last_p2); /* Ни одна мишень не повторяется в следующем цикле */

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

        p_work.timer_game++;
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

            while ((HAL_GetTick() - t_start) <= 400)
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
                if (p_work.bal < p_work.number_cycles)
                {
                    p_work.bal++;
                }
                cicl_mem++;
                Zumer();

                while (chekInputMem(GPIOA, exp1) || chekInputMem(GPIOA, exp2))
                {
                    HAL_Delay(10);
                }
                off_all();

                if (cicl_mem > p_work.number_cycles)
                {
                    step = 5;
                    ZumerOk();
                }
            }
            else
            {
                step = 5;
                ZumerError();
            }
            return;
        }
    }
}

/*
 * ==============================================================================
 * DVA: Зажигаются 6 мишеней (4 разных цветов + 1 пара одного цвета).
 * Погасить пару одного цвета двумя руками одновременно.
 * ==============================================================================
 */
void randomNumberColorPair(void)
{
    srand(time_random + (uint8_t)HAL_GetTick());
    uint8_t colors[5] = {0, 1, 2, 3, 4};
    uint8_t deck_idx = 5;
    uint8_t last_dup = 255;

    for (uint8_t i = 1; i <= p_work.number_cycles; i++)
    {
        if (deck_idx >= 5)
        {
            /* Алгоритм Фишера-Йетса для колоды из 5 цветов: исключает частые повторы */
            for (uint8_t k = 4; k > 0; k--)
            {
                uint8_t r = rand() % (k + 1);
                uint8_t tmp = colors[k];
                colors[k] = colors[r];
                colors[r] = tmp;
            }
            /* Гарантируем, что первый цвет новой колоды не равен последнему предыдущей */
            if (colors[0] == last_dup)
            {
                uint8_t tmp = colors[0];
                colors[0] = colors[1];
                colors[1] = tmp;
            }
            deck_idx = 0;
        }

        uint8_t dup_color = colors[deck_idx++];
        last_dup = dup_color;

        color_pair_one[i] = dup_color;
        color_pair_two[i] = dup_color + 5;

        /* Ровно одна пара цвета dup_color (2 мишени) */
        uint16_t mask = (1U << dup_color) | (1U << (dup_color + 5));

        /* Для остальных 4 цветов: строго по 1 мишени каждого цвета (всего 6 мишеней) */
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

    if (chekInputBotton(GPIOA, t1))
    {
        dva_latch1 = 1;
        setLedTarget(t1, 0); /* Мгновенно гасим мишень при ударе */
    }
    if (chekInputBotton(GPIOA, t2))
    {
        dva_latch2 = 1;
        setLedTarget(t2, 0); /* Мгновенно гасим мишень при ударе */
    }

    if (dva_latch1 && dva_latch2 && dva_scored_cycle != counter_number)
    {
        dva_scored_cycle = counter_number;
        if (p_work.bal < p_work.number_cycles)
        {
            p_work.bal++;
        }
        color_hit_done = 1;
        off_all();
        flag_beg = 10;
        p_work.timer_game = p_work.period;
        Zumer();
    }
}

void color_pair(void)
{
    if (p_work.timer_game >= p_work.period)
    {
        p_work.timer_game = 0;
        counter_number++;
        color_hit_done = 0;
        dva_latch1 = 0;
        dva_latch2 = 0;
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
            HAL_Delay(15);
            if (READ_BIT(GPIOA->IDR, (1U << t1)) && READ_BIT(GPIOA->IDR, (1U << t2)))
            {
                off_all();
                dva_latch1 = 0;
                dva_latch2 = 0;
                flag_beg = 0;
            }
        }
    }
}
