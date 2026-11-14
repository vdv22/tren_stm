#include "test.h"
#include "new_modes.h"

void testButton (void)
{
    /* В режиме теста TST: при нажатии на любую мишень загорается её светодиод,
       при отпускании светодиод гаснет. Поддерживается любое количество одновременно нажатых мишеней. */
    setLedTarget(0, (in_one() == 0) ? 1 : 0);
    setLedTarget(1, (in_two() == 0) ? 1 : 0);
    setLedTarget(2, (in_three() == 0) ? 1 : 0);
    setLedTarget(3, (in_four() == 0) ? 1 : 0);
    setLedTarget(4, (in_five() == 0) ? 1 : 0);
    setLedTarget(5, (in_six() == 0) ? 1 : 0);
    setLedTarget(6, (in_seven() == 0) ? 1 : 0);
    setLedTarget(7, (in_eight() == 0) ? 1 : 0);
    setLedTarget(8, (in_nine() == 0) ? 1 : 0);
    setLedTarget(9, (in_ten() == 0) ? 1 : 0);
}
