#ifndef INC_NEW_MODES_H_
#define INC_NEW_MODES_H_

#include "main.h"

/*
 * ==============================================================================
 * Окно одновременного нажатия двумя руками (в миллисекундах)
 * По умолчанию: 200 мс (согласно ТЗ)
 * ==============================================================================
 */
#define SIMULTANEOUS_PRESS_WINDOW_MS  200

/*
 * Вспомогательная функция определения цвета мишени и парной мишени того же цвета.
 * На физической панели тренажера 10 мишеней распаяны попарно:
 * Цвет 0: Мишени 1 и 2 (индексы 0 и 1)
 * Цвет 1: Мишени 3 и 4 (индексы 2 и 3)
 * Цвет 2: Мишени 5 и 6 (индексы 4 и 5)
 * Цвет 3: Мишени 7 и 8 (индексы 6 и 7)
 * Цвет 4: Мишени 9 и 10 (индексы 8 и 9)
 * Формулы:
 * color = target / 2
 * partner = target ^ 1
 */
static inline uint8_t getTargetColor(uint8_t t)
{
    return t / 2;
}

static inline uint8_t getPartnerTarget(uint8_t t)
{
    return t ^ 1;
}

/* Вспомогательные функции управления светодиодами */
void setLedTarget(uint8_t target, uint8_t state);
void onLed8On2Off(uint8_t off1, uint8_t off2);
void onLedColorPair(uint8_t cycle);

extern uint8_t rand_beg3_lit[51];
extern uint8_t rand_beg4_lit1[51], rand_beg4_lit2[51];
extern uint8_t rand_inv2_one[51], rand_inv2_two[51];
extern uint8_t rand_inv3_dark[51];
extern uint8_t rand_inv4_dark1[51], rand_inv4_dark2[51];

/* BEG3: Загорается 1 мишень -> зажечь мишень такого же цвета */
void randomNumberBeg3(void);
void beg3(void);
void checkReleaseBeg3(void);
void pollBeg3Buttons(void);

/* BEG4: Загораются 2 мишени -> зажечь 2 парные мишени двумя руками одновременно */
void randomNumberBeg4(void);
void beg4(void);
void checkReleaseBeg4(void);
void pollBeg4Buttons(void);

/* INV2: Горят 8 мишеней кроме двух разного цвета -> зажечь обе двумя руками одновременно */
void randomNumberInvDouble(void);
void inv_double(void);
void checkReleaseInvDouble(void);
void pollInvDoubleButtons(void);

/* INV3: Горят 9 мишеней кроме одной -> погасить мишень того же цвета, что и негорящая */
void randomNumberInv3(void);
void inv3(void);
void checkReleaseInv3(void);
void pollInv3Buttons(void);

/* INV4: Горят 8 мишеней кроме двух разного цвета -> погасить мишени парные негорящим двумя руками */
void randomNumberInv4(void);
void inv4(void);
void checkReleaseInv4(void);
void pollInv4Buttons(void);

/* MEM2: Память пар мишеней -> повторить комбинацию двумя руками одновременно */
void randomNumberMemDouble(void);
void mem_double_timer(void);
void checkInputMemDouble(void);

/* DVA: Зажигаются 6 мишеней (4 разных цвета + 1 пара) -> погасить пару одного цвета двумя руками */
void randomNumberColorPair(void);
void color_pair(void);
void checkReleaseColorPair(void);
void pollColorPairButtons(void);

void reset_all_mode_latches(void);

#endif /* INC_NEW_MODES_H_ */
