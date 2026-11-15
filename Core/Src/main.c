/* USER CODE BEGIN Header */
/**

  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim10;

/* USER CODE BEGIN PV */
char *str;
uint8_t adress_i2c,ledon=1 ,adr[2],flag_beg=0;
uint16_t a,count_timer=0;
const uint8_t temp_numder[6]={2,3,5,10,20,50};
uint8_t time_random,random_numder[50],start_play=0,counter_number=0,flag_zumer=0,count_zumer=0,count_mem=0,rand_number_duble_one[50],rand_number_duble_two[50];
uint8_t position=0, flag_onoff=0,mem_stop=0,result=0,cicl_mem=0;
uint8_t prob1=0;
uint8_t step=5 ;  // переменная шага программы.
struct adc_k button;
param p_work;
int MyDataOne=823,prob=0;   //  первая переменная для записи
int MyDataTwo=560;          //  вторая  переменная для записи
int *p= &MyDataOne;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();
  HAL_TIM_Base_Start_IT(&htim10);
  __HAL_TIM_ENABLE_IT(&htim10, TIM_IT_UPDATE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  off_all();
  if (read_adcn() < 5)
  {
      HAL_Delay(100);
      if (read_adcn() < 5)
      {
          Setbotton();
      }
  }
  HAL_Delay(500);
  SystemMem();
  indication();
  if (p_work.mode != test)
  {
      on_one();       /* Зажигаем стартовую мишень №1 при включении питания */
      start_play = 0; /* Готовность к старту игры по нажатию на мишень №1 */
  }
  else
  {
      off_all();
      start_play = 0;
  }

while (1)
{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    static uint8_t last_bal = 255;
    if (p_work.bal != last_bal)
    {
        lcd_goto(14, 1);
        lcd_number(p_work.bal);
        if (p_work.bal < 10)
        {
            lcd_string(" ");
        }
        last_bal = p_work.bal;
    }

    /* Кнопка включения/выключения питания (PB10, active-low) */
    if (flag_onoff == 1 || (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET))
    {
        HAL_Delay(30); /* Антидребезг */
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET || flag_onoff == 1)
        {
            start_play = 3;
            ledon = 0;
            lcd_clear();
            p_work.bal = 0;
            off_all();

            /* Ждём полного отпускания кнопки выключения */
            while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET)
            {
                HAL_Delay(10);
            }
            HAL_Delay(100);

            /* Режим ожидания/выключено: ждём повторного нажатия для включения */
            while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET)
            {
                HAL_Delay(50);
            }

            HAL_Delay(50); /* Антидребезг нажатия на включение */

            /* Ждём отпускания */
            while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET)
            {
                HAL_Delay(10);
            }
            HAL_Delay(100);

            flag_onoff = 0; /* СБРОС ФЛАГА: предотвращает повторный вход в цикл */
            ledon = 1;
            indication();
            if (p_work.mode != test)
            {
                on_one();
            }
            start_play = 0;
            step = 0;
            last_bal = 255;
        }
        else
        {
            flag_onoff = 0;
        }
    }

	if (flag_zumer == 1)
	{
		p_work.bal == p_work.number_cycles ? ZumerOk() : ZumerError();
		flag_zumer = 0;
		if (p_work.mode != test)
		{
			on_one();
		}
		start_play = 0; /* Auto-arm Target 1 for next round */
	}
	if(p_work.mode==game_invers || p_work.mode==game_beg)
	{
		proverka(flag_beg);//���� ������ �������� �� ��������  flag_beg � ���� ���� ����� ������
	}

	if((out(p_work.duble_one_flag,p_work.duble_two_flag))==2 && p_work.mode==game_duble)
	{
		flag_beg=0;
	}

	if(p_work.mode==game_beg3)
	{
		checkReleaseBeg3();
	}

	if(p_work.mode==game_beg4)
	{
		checkReleaseBeg4();
	}

	if(p_work.mode==game_inv_double)
	{
		checkReleaseInvDouble();
	}

	if(p_work.mode==game_inv3)
	{
		checkReleaseInv3();
	}

	if(p_work.mode==game_inv4)
	{
		checkReleaseInv4();
	}

	if(p_work.mode==game_color_pair)
	{
		checkReleaseColorPair();
	}

	if(p_work.mode==test)
	{
		testButton();
	}

	if (start_play == 1 && flag_beg == 0)
	{
		if (p_work.mode == game_beg)
		{
			chekBotton(random_numder[counter_number]);
		}
		else if (p_work.mode == game_invers)
		{
			chekBottonInvert(random_numder[counter_number]);
		}
	}

  if(bt_ok()) SetParametrPlay();

  if(!(in_one()) && start_play==0 && p_work.mode != test && p_work.mode != mem_game && p_work.mode != game_mem_double)  // если нажата первая кнопка
	{
	      if(p_work.mode==game_invers || p_work.mode==game_beg)
	    	  	RandomNumber();
	      else if(p_work.mode==game_duble)
				randomNumberDuble();
	      else if(p_work.mode==game_beg3)
				randomNumberBeg3();
	      else if(p_work.mode==game_beg4)
				randomNumberBeg4();
	      else if(p_work.mode==game_inv_double)
				randomNumberInvDouble();
	      else if(p_work.mode==game_inv3)
				randomNumberInv3();
	      else if(p_work.mode==game_inv4)
				randomNumberInv4();
	      else if(p_work.mode==game_color_pair)
				randomNumberColorPair();

	   	  counter_number=1;
	      p_work.timer_game=0;
	      p_work.bal=0;
	      last_bal = 255;
	      flag_beg=0;
	      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	      while(!(in_one())) { HAL_Delay(5); }
	      HAL_Delay(30);
	      while(!(in_one()));

	      /* Зажигаем мишени первого такта СТРОГО ПРИ ОТПУСКАНИИ для всех режимов */
	      if (p_work.mode == game_beg)
	      {
	          onLedBeg(random_numder[1]);
	      }
	      else if (p_work.mode == game_duble)
	      {
	          onLedDouble(rand_number_duble_one[1], rand_number_duble_two[1]);
	      }
	      else if (p_work.mode == game_beg3)
	      {
	          setLedTarget(rand_beg3_lit[1], 1);
	      }
	      else if (p_work.mode == game_beg4)
	      {
	          setLedTarget(rand_beg4_lit1[1], 1);
	          setLedTarget(rand_beg4_lit2[1], 1);
	      }
	      else if (p_work.mode == game_invers)
	      {
	          if (COUNT_LED == 10) off_all_invert(); else off_all_invert_half();
	      }
	      else if (p_work.mode == game_inv_double)
	      {
	          onLed8On2Off(rand_inv2_one[1], rand_inv2_two[1]);
	      }
	      else if (p_work.mode == game_inv3)
	      {
	          for (uint8_t i = 0; i < 10; i++) setLedTarget(i, (i != rand_inv3_dark[1]) ? 1 : 0);
	      }
	      else if (p_work.mode == game_inv4)
	      {
	          onLed8On2Off(rand_inv4_dark1[1], rand_inv4_dark2[1]);
	      }
	      else if (p_work.mode == game_color_pair)
	      {
	          onLedColorPair(1);
	      }

	      start_play=1;                                         // готовность к запуску
	  }

   if(time_random>50) {time_random=0;} else {time_random++;}       // ��������� Srand

          if(bt_reset())
          {
              uint32_t press_start = HAL_GetTick();
              uint8_t mode_entered = 0;

              while (bt_reset())
              {
                  HAL_Delay(10);
                  if ((HAL_GetTick() - press_start) >= 800)
                  {
                      SetMode();
                      mode_entered = 1;
                      break;
                  }
              }

              if (!mode_entered)
              {
                  /* Короткое нажатие: сброс игры, сброс счёта, взведение мишени №1 */
                  start_play = 3;
                  off_all();
                  p_work.bal = 0;
                  last_bal = 255;
                  lcd_goto(14, 1);
                  lcd_string("0 ");
                  if (p_work.mode != test)
                  {
                      on_one();
                  }
                  start_play = 0;
                  step = 0;
              }

              while (bt_reset()) { HAL_Delay(10); }
              HAL_Delay(50);
          }

    if ((step==0 || step==5) &&  p_work.mode ==mem_game && !in_one()   )  // если выбран режим память , и нажата кнопка пуск
    {
    	RandomNumber();
    	p_work.bal=0;
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
    	while(!(in_one())) { HAL_Delay(5); }
    	HAL_Delay(30);
    	while(!(in_one()));
    	step=1          ;
    	start_play=1    ;
    	counter_number=1;
    	off_all() ;
    	cicl_mem=1;
    	result=60;
    	p_work.timer_game=0;
    }

    if ((step==0 || step==5) &&  p_work.mode ==game_mem_double && !in_one()   )  // если выбран режим память парами, и нажата кнопка пуск
    {
    	randomNumberMemDouble();
    	p_work.bal=0;
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
    	while(!(in_one())) { HAL_Delay(5); }
    	HAL_Delay(30);
    	while(!(in_one()));
    	step=1          ;
    	start_play=1    ;
    	counter_number=1;
    	off_all() ;
    	cicl_mem=1;
    	result=60;
    	p_work.timer_game=0;
    }
////////////////////  работа в режиме память///////////////////////////
    if  ( p_work.mode ==mem_game && step==2 )
    {
    	 for (int i=0;i<= 9 ; i++)
    		 {
    	         if (chekInputMem( GPIOA,i ) )
    			   {result=i;
    			     break ;
    			   }
    	         else
    	        	 result=60;
    		 }

    	 if ( result !=60 )
    	 {
    		 if ( result == random_numder[cicl_mem ] )
    		 {
    			 onLedBeg(result);
    			 p_work.bal ++;
    			 cicl_mem++;
    			 Zumer();
    			 while( READ_BIT(GPIOA->IDR,0x1UL << result)==RESET )
    			     {}
    			  off_all();

    		 }
    		 else
    		 {
    			 step=5;
                ZumerError();
    		 }
         }

    	 if ( p_work.number_cycles  == p_work.bal )
    	 {
    		 lcd_goto(14,1);
    		 lcd_number( p_work.bal );
    		 step=5;
    		 ZumerOk();
    	 }
   }

    if ( p_work.mode == game_mem_double && step == 2 )
    {
    	checkInputMemDouble();
    }

////////////////////////////////////////////

  HAL_Delay(10);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_10B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 41999;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 199;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, out5_Pin|zumer_Pin|out6_Pin|out7_Pin
                          |out8_Pin|out9_Pin|out10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, out1_Pin|out2_Pin|out3_Pin|out4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : in1_Pin in2_Pin in3_Pin in4_Pin
                           in5_Pin in6_Pin in7_Pin in8_Pin
                           in9_Pin in10_Pin */
  GPIO_InitStruct.Pin = in1_Pin|in2_Pin|in3_Pin|in4_Pin
                          |in5_Pin|in6_Pin|in7_Pin|in8_Pin
                          |in9_Pin|in10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : out5_Pin zumer_Pin out6_Pin out7_Pin
                           out8_Pin out9_Pin out10_Pin */
  GPIO_InitStruct.Pin = out5_Pin|zumer_Pin|out6_Pin|out7_Pin
                          |out8_Pin|out9_Pin|out10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : out1_Pin out2_Pin out3_Pin out4_Pin */
  GPIO_InitStruct.Pin = out1_Pin|out2_Pin|out3_Pin|out4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
