#include "memory.h"
extern param p_work;
extern const uint8_t temp_numder[7];
uint8_t read_memory (uint8_t data)
{
	uint8_t  data_out;
	HAL_I2C_Mem_Read(&hi2c1, 80<<1, data, I2C_MEMADD_SIZE_16BIT,&data_out , 1,HAL_MAX_DELAY );

return data_out;
}

void write_memory (uint8_t addr,uint8_t data)
{
HAL_I2C_Mem_Write(&hi2c1, 80<<1, addr, I2C_MEMADD_SIZE_16BIT,&data , 1,HAL_MAX_DELAY);
}

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

void write_memory_adc (uint8_t adresMem,uint16_t dataMem)
{
   uint8_t adr=80;
   uint8_t out[3];
 if(dataMem<=255)
 {
	 out[0]=dataMem;
	 out[1]=0;
	 out[2]=0;
	 HAL_I2C_Mem_Write(&hi2c1, adr<<1, adresMem, I2C_MEMADD_SIZE_16BIT, out ,3,HAL_MAX_DELAY);
 }
 else
 {
	out[0]=dataMem/255;
	out[1]=dataMem-(255*out[0]);
	out[2]=1;
	HAL_I2C_Mem_Write(&hi2c1, adr<<1, adresMem, I2C_MEMADD_SIZE_16BIT, out ,3,HAL_MAX_DELAY);
 }
}

uint16_t read_memory_adc (uint8_t adrress)
{
	uint16_t out=0;
	if(read_memory(adrress+2)==0)
	{
		return  read_memory (adrress);
	}
   else
    {
       out=(255 * read_memory(adrress) ) + read_memory(adrress+1);
       return out;
    }
}

void SystemMem (void)
{
	 button._minus = read_memory_adc(200);
     button._plus  = read_memory_adc(20);
	 button._reset = read_memory_adc(30);
     p_work.mode   = read_memory(40);                    //читаем режим тренировки
	 p_work.number_cycles= temp_numder[read_memory(41)]; //количество циклов
	 p_work.period= read_memory(100);                    // время периодов

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
}
