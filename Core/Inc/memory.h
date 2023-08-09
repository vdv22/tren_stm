#ifndef INC_MEMORY_H_
#define INC_MEMORY_H_
#include "main.h"
extern struct adc_k button;
ADC_HandleTypeDef hadc1;
uint8_t read_memory(uint8_t data);
void write_memory(uint8_t addr, uint8_t data);
uint16_t read_adcn(void);
void write_memory_adc(uint8_t adresMem, uint16_t dataMem);
uint16_t read_memory_adc(uint8_t adrress);
void SystemMem(void);
#define  on_one()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET) //1
#define  on_two()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET) //2
#define  on_three()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET) //3
#define  on_four()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET) //4
#define  on_five()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)  //5
#define  on_six()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)  //6
#define  on_seven()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)  //7
#define  on_eight()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)  //8
#define  on_nine()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)  //9
#define  on_ten()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)  //10
//////////////////////

////////////////////////////////////////////////////////////////////////////////////
#define  off_all()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET),\
                       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET),\
					   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
////////////////////////////////////////////////////////////////////////////////////

#define in_one()        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)
#define in_two()        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)
#define in_three()      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)
#define in_four()       HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)
#define in_five()       HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)
#define in_six()        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)
#define in_seven()      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)
#define in_eight()      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)
#define in_nine()       HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)
#define in_ten()        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)

#define bt_reset()    (read_adcn()>=button._reset-10) && (read_adcn()<=button._reset+10) // ����� reset
#define bt_plus()     (read_adcn()>=button._plus-10) && (read_adcn()<=button._plus+10)
#define bt_minus()    (read_adcn()>=button._minus-10) && (read_adcn()<=button._minus+10)
#define bt_ok()       read_adcn()<5
////////////////////////////////////////////////////////////////
enum game {
	game_beg = 0, game_invers, game_duble, test, game_duble_color
};
////////////////////////////////////////////////

struct adc_k {
	uint16_t _minus;
	uint16_t _plus;
	uint16_t _ok;
	uint16_t _reset;
};

typedef struct        // ��������� ���������� ����
{
	uint8_t number_cycles;   // ���������� ����������
	uint8_t period;          // ��������
	uint8_t bal;             //��������� ������
	uint8_t mode;   //   ����� 1 - beg  2 -inv 3 -dub 4-dub color
	uint8_t timer_game :4; //   ������� ���������� ������� � ���������� � �������� 1 ��.
	uint8_t duble_one_trye :1; // ���� �������� �� ������ ������� �����
	uint8_t duble_two_trye :1; // ���� �������� �� ����� �������� �����
	uint8_t duble_one_flag; // ���� ������ ��������
	uint8_t duble_two_flag; //���� ������ ��������
} param;
#endif /* INC_MEMORY_H_ */
