#include "memory.h"
extern param p_work;
FLASH_EraseInitTypeDef MyStruct;
uint32_t errors;
extern const uint8_t temp_numder[6];
extern uint8_t position;

uint16_t read_adcn (void)
{
	uint32_t sum = 0;
	HAL_ADC_Start(&hadc1);
	for (uint8_t i = 0; i < 4; i++)
	{
		if (HAL_ADC_PollForConversion(&hadc1, 2) == HAL_OK)
		{
			sum += HAL_ADC_GetValue(&hadc1);
		}
		else
		{
			sum += 1023;
		}
		if (i < 3)
		{
			HAL_ADC_Start(&hadc1);
		}
	}
	HAL_ADC_Stop(&hadc1);
	return (uint16_t)(sum / 4);
}

uint8_t bt_ok(void)
{
	return (read_adcn() < 35) ? 1 : 0;
}

#define ADC_TOLERANCE 80

static uint8_t check_adc_match(uint16_t adc, uint16_t target, uint16_t range_min, uint16_t range_max)
{
	if (target >= 20 && target <= 1000)
	{
		uint16_t low = (target > ADC_TOLERANCE) ? (target - ADC_TOLERANCE) : 0;
		uint16_t high = target + ADC_TOLERANCE;
		if (adc >= low && adc <= high)
		{
			return 1;
		}
	}
	if (adc >= range_min && adc <= range_max)
	{
		return 1;
	}
	return 0;
}

uint8_t bt_reset(void)
{
	uint16_t adc = read_adcn();
	/* White button: RESET (~823, window 720..950) */
	return check_adc_match(adc, button._reset, 720, 950);
}

uint8_t bt_plus(void)
{
	uint16_t adc = read_adcn();
	/* Green button: PLUS (~560, window 420..680) */
	return check_adc_match(adc, button._plus, 420, 680);
}

uint8_t bt_minus(void)
{
	uint16_t adc = read_adcn();
	/* Blue button: MINUS (~250, window 50..380) */
	return check_adc_match(adc, button._minus, 50, 380);
}

void SystemMem (void)
{
	button._minus = FLASH_Read(ADDRESS_MINUS);
	button._plus  = FLASH_Read(ADDRESS_PLUS);
	button._reset = FLASH_Read(ADDRESS_RESET);
	p_work.mode   = FLASH_Read(ADDRESS_MODE);
	position      = FLASH_Read(ADDRESS_CICLES);
	p_work.period = FLASH_Read(ADDRESS_PERIOD);

	/* Защита от пустой/стёртой Flash памяти (значения 0xFFFF / 0xFF) */
	if (position >= 6)
	{
		position = 3; /* По умолчанию 10 циклов (индекс 0: temp_numder[0] = 2) */
	}
	p_work.number_cycles = temp_numder[position];

	if (p_work.mode > 11)
	{
		p_work.mode = game_beg; /* Режим по умолчанию: BEG */
	}

	if (p_work.period < 2 || p_work.period > 20)
	{
		p_work.period = 15; /* Темп по умолчанию: 1.5 сек */
	}

	/* Значения АЦП кнопок по умолчанию, если память чистая/не откалибрована */
	if (button._minus > 1010 || button._minus < 10)
	{
		button._minus = 250;
	}
	if (button._plus > 1010 || button._plus < 10)
	{
		button._plus = 560;
	}
	if (button._reset > 1010 || button._reset < 10)
	{
		button._reset = 823;
	}
}

uint16_t FLASH_Read(uint32_t address)
{
    return (*(__IO uint16_t*)address);
}

void  int_mem_write (void)
{
	  MyStruct.Sector=5;                                	//  выбираем сектор для стирания
	  MyStruct.NbSectors=1;									// выбираем кол-во секторов
	  MyStruct.TypeErase=FLASH_TYPEERASE_SECTORS;       	// тип стирания.
	  HAL_FLASH_Unlock();
	  HAL_FLASHEx_Erase(&MyStruct, &errors);    				//  стирание флэш
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,ADDRESS_MINUS,button._minus);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,ADDRESS_PLUS,button._plus);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,ADDRESS_RESET,button._reset);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,ADDRESS_MODE,p_work.mode);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,ADDRESS_PERIOD,p_work.period);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,ADDRESS_CICLES,position);

	  HAL_FLASH_Lock();                             			// блокируем доступ к памяти
}
