#include "memory.h"
extern param p_work;
FLASH_EraseInitTypeDef MyStruct;
uint32_t errors;
extern const uint8_t temp_numder[7];
extern uint8_t position;

uint16_t read_adcn (void)
{
uint16_t our=0;
HAL_ADC_Start(&hadc1);
for(uint8_t i=0;i<4;i++)
{
	HAL_ADC_PollForConversion(&hadc1, 10);
	our=HAL_ADC_GetValue(&hadc1);
	our+=our;
}
our=our/2;
HAL_ADC_Stop(&hadc1);
return our;
}



void SystemMem (void)
{
	 button._minus = FLASH_Read(ADDRESS_MINUS);
     button._plus  = FLASH_Read(ADDRESS_PLUS);
	 button._reset = FLASH_Read(ADDRESS_RESET);
     p_work.mode   = FLASH_Read(ADDRESS_MODE);
     position      = FLASH_Read(ADDRESS_CICLES);
     p_work.number_cycles= temp_numder[ position];
	 p_work.period= FLASH_Read(ADDRESS_PERIOD);

	 /*
     if( p_work.number_cycles>100)
	 {
		 write_memory(41,0);
		 p_work.number_cycles=0;
	 }
	 if(p_work.mode>3)
	 {
		 write_memory(40,0);
		 p_work.mode=0;
	 }

	 if(read_memory(100)>20)
	 {
		 write_memory(100,1);
		 p_work.period=1;
	 }
	 */
}


uint32_t FLASH_Read(uint32_t address)
{
    return (*(__IO uint32_t*)address);
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
