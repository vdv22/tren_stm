
#ifndef INC_DOUBLECOLOR_H_
#define INC_DOUBLECOLOR_H_
#include "main.h"
#define WHITE()       	(on_one(),on_six())
#define YELOW()       	(on_two(),on_seven())
#define BLUE()  		(on_three(),on_eight())
#define GREEN()			(on_four(),on_nine())
#define RED()			(on_five(),on_ten())

void randomNumberDubleColor (void);
void onLedDoubleColor (uint8_t one,uint8_t two,uint8_t);
void dubleColor (void);
enum color
{
	white=0,
	yellow,
	blue,
	green,
	red
};
#endif /* INC_DOUBLECOLOR_H_ */
