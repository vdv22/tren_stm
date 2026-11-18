#include "memory.h"

extern param p_work;
extern const uint8_t temp_numder[6];
extern uint8_t position;
extern I2C_HandleTypeDef hi2c1;

FLASH_EraseInitTypeDef MyStruct;
uint32_t errors;

/* Массивы индивидуальных настроек темпа и количества для всех 12 игр */
uint16_t game_period[12];
uint16_t game_cicles[12];

/* ==============================================================================
 * Поддержка внешней EEPROM I2C: AT24C02B (256 байт, адрес 0xA0)
 * ============================================================================== */
#define AT24C02_ADDR        0xA0
#define AT24C02_SIG_ADDR    0x00
#define AT24C02_SIGNATURE   0xA5
#define AT24C02_MODE_ADDR   0x01
#define AT24C02_PERIOD_BASE 0x02
#define AT24C02_CICLES_BASE 0x10

uint8_t at24c02_is_present(void)
{
    return (HAL_I2C_IsDeviceReady(&hi2c1, AT24C02_ADDR, 2, 5) == HAL_OK);
}

void at24c02_write_byte(uint8_t addr, uint8_t data)
{
    if (at24c02_is_present())
    {
        HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
        HAL_Delay(5); /* Задержка записи EEPROM ~5мс */
    }
}

uint8_t at24c02_read_byte(uint8_t addr)
{
    uint8_t data = 0xFF;
    if (at24c02_is_present())
    {
        HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    }
    return data;
}

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

uint16_t FLASH_Read(uint32_t address)
{
    return (*(__IO uint16_t*)address);
}

void SystemMem (void)
{
	button._minus = FLASH_Read(ADDRESS_MINUS);
	button._plus  = FLASH_Read(ADDRESS_PLUS);
	button._reset = FLASH_Read(ADDRESS_RESET);
	p_work.mode   = FLASH_Read(ADDRESS_MODE);

	if (p_work.mode > 11)
	{
		p_work.mode = game_beg; /* Режим по умолчанию: BEG */
	}

	/* Значения АЦП кнопок по умолчанию при чистой Flash */
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

	/* Чтение индивидуальных настроек для каждой из 12 игр */
	uint8_t eeprom_valid = 0;
	if (at24c02_is_present() && at24c02_read_byte(AT24C02_SIG_ADDR) == AT24C02_SIGNATURE)
	{
		eeprom_valid = 1;
	}

	for (uint8_t m = 0; m < 12; m++)
	{
		if (eeprom_valid)
		{
			game_period[m] = at24c02_read_byte(AT24C02_PERIOD_BASE + m);
			game_cicles[m] = at24c02_read_byte(AT24C02_CICLES_BASE + m);
		}
		else
		{
			game_period[m] = FLASH_Read(ADDRESS_GAME_PERIOD_BASE + m * 2);
			game_cicles[m] = FLASH_Read(ADDRESS_GAME_CICLES_BASE + m * 2);
		}

		/* Валидация количества циклов (индекс 0..5) */
		if (game_cicles[m] >= 6)
		{
			game_cicles[m] = 3; /* По умолчанию 10 циклов (индекс 3: 10) */
		}

		/* Валидация темпа (2..20 -> 0.2с .. 2.0с) */
		if (game_period[m] < 2 || game_period[m] > 20)
		{
			game_period[m] = 10; /* По умолчанию 1.0 сек */
		}
	}

	/* Загрузка активных параметров для текущей выбранной игры */
	position = (uint8_t)game_cicles[p_work.mode];
	p_work.number_cycles = temp_numder[position];
	p_work.period = (uint8_t)game_period[p_work.mode];
}

void int_mem_write (void)
{
	  /* Проверяем, изменились ли данные во Flash */
	  uint8_t changed = 0;
	  if (FLASH_Read(ADDRESS_MODE) != (uint16_t)p_work.mode ||
	      FLASH_Read(ADDRESS_PERIOD) != (uint16_t)p_work.period ||
	      FLASH_Read(ADDRESS_CICLES) != (uint16_t)position ||
	      FLASH_Read(ADDRESS_MINUS) != button._minus ||
	      FLASH_Read(ADDRESS_PLUS) != button._plus ||
	      FLASH_Read(ADDRESS_RESET) != button._reset)
	  {
		  changed = 1;
	  }

	  for (uint8_t m = 0; m < 12; m++)
	  {
		  if (FLASH_Read(ADDRESS_GAME_PERIOD_BASE + m * 2) != game_period[m] ||
		      FLASH_Read(ADDRESS_GAME_CICLES_BASE + m * 2) != game_cicles[m])
		  {
			  changed = 1;
			  break;
		  }
	  }

	  /* Сохраняем в микросхему AT24C02B, если она подключена */
	  if (at24c02_is_present())
	  {
		  at24c02_write_byte(AT24C02_SIG_ADDR, AT24C02_SIGNATURE);
		  at24c02_write_byte(AT24C02_MODE_ADDR, p_work.mode);
		  for (uint8_t m = 0; m < 12; m++)
		  {
			  at24c02_write_byte(AT24C02_PERIOD_BASE + m, (uint8_t)game_period[m]);
			  at24c02_write_byte(AT24C02_CICLES_BASE + m, (uint8_t)game_cicles[m]);
		  }
	  }

	  if (!changed)
	  {
		  return; /* Данные во Flash идентичны: пропускаем стирание сектора */
	  }

	  MyStruct.Sector = 5;                              // Выбираем сектор 5 (128 КБ)
	  MyStruct.NbSectors = 1;                           // 1 сектор
	  MyStruct.TypeErase = FLASH_TYPEERASE_SECTORS;     // Стирание сектора
	  HAL_FLASH_Unlock();
	  HAL_FLASHEx_Erase(&MyStruct, &errors);            // Стирание сектора

	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_MINUS, button._minus);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_PLUS, button._plus);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_RESET, button._reset);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_MODE, p_work.mode);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_PERIOD, p_work.period);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_CICLES, position);

	  /* Запись таблицы индивидуальных настроек для всех 12 игр */
	  for (uint8_t m = 0; m < 12; m++)
	  {
		  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_GAME_PERIOD_BASE + m * 2, game_period[m]);
		  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDRESS_GAME_CICLES_BASE + m * 2, game_cicles[m]);
	  }

	  HAL_FLASH_Lock();                                 // Блокируем Flash
}
