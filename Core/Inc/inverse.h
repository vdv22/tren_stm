#ifndef INC_INVERSE_H_
#define INC_INVERSE_H_
#include "main.h"
void invert (void);
void ifBottonOnInvert (uint8_t n);
void onLedInvert(uint8_t on);
void chekBottonInvert (uint8_t bot);

#define  on_one_invert()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET) //1
#define  on_two_invert()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET) //2
#define  on_three_invert()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET) //3
#define  on_four_invert()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET) //4
#define  on_five_invert()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)  //5
#define  on_six_invert()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)  //6
#define  on_seven_invert()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)  //7
#define  on_eight_invert()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)  //8
#define  on_nine_invert()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)  //9
#define  on_ten_invert()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)  //10
/////////////////////////////////////////////////////////////////////////////////////////////
#define  off_all_invert() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET),\
                          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET),\
					      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)

#define off_all_invert_half()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET),\
                               HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET),\
					           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET),\
					           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET),\
					           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET),\
							   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET),\
							   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET),\
							   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET),\
							   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET),\
							   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)


#endif /* INC_INVERSE_H_ */
