#include"i2c.h"
extern uint8_t ledon,adress_i2c;
uint8_t stringS=4;
void write_comanda(uint8_t comanda)      //������- �������� �������.
{
	uint8_t st,ml,tmp; //��������� ����������.
	uint8_t *ptmp=&tmp;
	st=(comanda & 0xf0);
	ml=((comanda<<4)& 0xf0);
	if (ledon==1)
	{
		tmp=(st |0x0C);
	}
	else
	{
		tmp=(st |0x04);
	}
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);
    if (ledon==1)
	{
		tmp=tmp&0xFB;
	}
	else
	{
		tmp=tmp&0xF3;
	}
    HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);

	if (ledon==1)
	{
		tmp=(ml |0x0C);
	}
	else
	{
		tmp=(ml |0x04);
	}
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);

	if (ledon==1)
	{
		tmp=tmp&0xFB;
	}
	else
	{
		tmp=tmp&0xF3;
	}
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);
}

void write_data (uint8_t data)               //�������� ������.
{  uint8_t st,ml,tmp;
   uint8_t *ptmp=&tmp;
	st=(data & 0xf0);
	ml=((data<<4)& 0xF0);
 if (ledon==1)
	{
		tmp=(st |0x0D);
	}
	else
	{
		tmp=(st |0x05);
	}
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);
  if (ledon==1)
	{
		tmp=tmp&0xfB;
	}
	else
	{
		tmp=tmp&0xf3;
	}
  HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);

	if (ledon==1)
	{tmp=(ml |0x0d);
	}
	else
	{
		tmp=(ml |0x05);
	}
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);

	if (ledon==1)
	{
		tmp=tmp&0xfb;
	}
	else
	{
		tmp=tmp&0xf3;
	}
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1, ptmp, 1, HAL_MAX_DELAY);
}

void lcd_goto( uint8_t x, uint8_t y) // ������ �� (X,Y)
{
	if (stringS==2)
	{
		if (y==1)
		{
		x=64+x;}
	}

	if (stringS==4)
	{
		if (y==1)
		{
			x=(x+0x40);
		}

		if (y==2)
		{
			x=(x+0x14);
		}
		if(y==3)
		{
			x=x+0x54;
		}
	}
	x=x |0x80;
write_comanda(x);
}


void  lcd_string ( const char *streeng)
{
	for(uint8_t count=0;*streeng!='\0';count++)
	{
		write_data(*streeng);
        streeng++;
	}

}

void	lcd_number (int num)  // ������ �����
{   int tmp1;
	uint8_t Nmp;
	if (num<0)
	{
		lcd_string("-");
		num=(0-num);
	}
	tmp1=num;

	if (num>=10000)
	{
		Nmp=tmp1/10000;
		write_data(Nmp+48);
		tmp1=tmp1-(Nmp*10000);
	}

	if (num>=1000)
	{
		Nmp=tmp1/1000;
		write_data(Nmp+48);
		tmp1=tmp1-(Nmp*1000);
	}

	if (num>=100)
	{
		Nmp=tmp1/100;
		write_data(Nmp+48);
		tmp1=tmp1-(Nmp*100);
	}

	if (num>=10)
	{
		Nmp=(tmp1/10);
		write_data(Nmp+48);
		tmp1=tmp1-(Nmp*10);
	}
	write_data(tmp1+48);
}

void lcd_init (void) //�������������
{
	//uint8_t c=0;
	 for(uint8_t i=0,j;i<128;i++)
	  	   {
	  	 	  j=HAL_I2C_IsDeviceReady(&hi2c1, i<<1, 1, 10);
	            if((j==HAL_OK) && (i!=80))
	            {
	            		adress_i2c=i;
	            }
	  	   }

	uint8_t temp;
	uint8_t *point=&temp;
	HAL_Delay(15);
	temp=0x34;
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1,point , 1, HAL_MAX_DELAY);

	HAL_Delay(1);
	temp=0x30;
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1,point , 1, HAL_MAX_DELAY);
	HAL_Delay(5);
	temp=0x34;
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1,point, 1, HAL_MAX_DELAY);
	temp=0x30;
	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1,point, 1, HAL_MAX_DELAY);
	HAL_Delay(1);
	temp=0x24;
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1,point , 1, HAL_MAX_DELAY);
	HAL_Delay(1);
	temp=0x20;
	HAL_I2C_Master_Transmit(&hi2c1, adress_i2c<<1,point , 1, HAL_MAX_DELAY);

	HAL_Delay(1);
    write_comanda(0b00101000);
    HAL_Delay(1);
	write_comanda(0b00001001);

	HAL_Delay(1);
	write_comanda(0x01);
	HAL_Delay(2);

	write_comanda(0x06);
	HAL_Delay(1);

	write_comanda(0x0c); // ������ ������ 0xd,(_) 0x0e
	HAL_Delay(1);

	write_comanda(0x02);
	HAL_Delay(2);
	lcd_goto(0,0);
}

void lcd_clear (void)
{
lcd_goto(0, 0);
lcd_string("                ");
lcd_goto(0, 1);
lcd_string("                ");
}
