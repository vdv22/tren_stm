
#ifndef INC_I2C_H_
#define INC_I2C_H_
#include "main.h"
extern I2C_HandleTypeDef hi2c1;
void write_data (uint8_t data);
void lcd_goto( uint8_t x, uint8_t y);
void  lcd_string ( const char *streeng);
void	lcd_number (int num);
void lcd_init (void);
void lcd_clear (void);
#endif /* INC_I2C_H_ */
