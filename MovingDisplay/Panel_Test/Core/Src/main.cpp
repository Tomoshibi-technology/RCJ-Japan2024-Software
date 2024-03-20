/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812.h"
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
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch2;

/* USER CODE BEGIN PV */
//WS2812B Neopixel(&htim3, TIM_CHANNEL_2, &hdma_tim3_ch2);

//#define HIGH 10
//#define LOW 5
//#define NUM_PIXELS 256
//
//uint8_t rgb_buf[NUM_PIXELS][3];
//uint8_t wr_buf[48]={0};
//uint_fast8_t wr_buf_p = 0;
//uint8_t start_ready = 1;

WS2812 Neopixel(&htim3,TIM_CHANNEL_2,&hdma_tim3_ch2);
int k = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
//	   Neopixel.execute();
//}

/*
void set_color(uint8_t id, uint8_t r, uint8_t g,uint8_t b){
  rgb_buf[id][0]=r;
  rgb_buf[id][1]=g;
  rgb_buf[id][2]=b;
}

void clear(){
  for(int i=0; i<NUM_PIXELS; i++){
    rgb_buf[i][0]=0;
    rgb_buf[i][1]=0;
    rgb_buf[i][2]=0;
  }
}


void show(){
    if(wr_buf_p != 0 || hdma_tim3_ch2.State != HAL_DMA_STATE_READY){
        HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_2);
        wr_buf_p = 0;
        for(uint8_t i=0; i <48; i++){wr_buf[i] = 0;}
        return;
    }
    for(uint_fast8_t i = 0; i < 8; i++){
        wr_buf[i   ] = 0;
        wr_buf[i+ 8] = 0;
        wr_buf[i+16] = 0;

        wr_buf[i+24] = 0;
        wr_buf[i+32] = 0;
        wr_buf[i+40] = 0;
    }
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2, (uint32_t *)wr_buf, 48);
    HAL_Delay(8);
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  // DMA buffer set from LED(wr_buf_p) to LED(wr_buf_p + 1)
  if(wr_buf_p < NUM_PIXELS) {
    for(uint_fast8_t i = 0; i < 8; i++) {
      wr_buf[i     ] = ((rgb_buf[wr_buf_p][1]>>(7-i))&1) ? HIGH : LOW;
      wr_buf[i +  8] = ((rgb_buf[wr_buf_p][0]>>(7-i))&1) ? HIGH : LOW;
      wr_buf[i + 16] = ((rgb_buf[wr_buf_p][2]>>(7-i))&1) ? HIGH : LOW;
    }
    wr_buf_p++;
  } else if (wr_buf_p < NUM_PIXELS + 2) {
    for(uint8_t i = 0; i < 24; i++){ wr_buf[i] = 0;};
    wr_buf_p++;
  }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  if(wr_buf_p < NUM_PIXELS) {
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i + 24] = ((rgb_buf[wr_buf_p][1]>>(7-i))&1) ? HIGH : LOW;
      wr_buf[i + 32] = ((rgb_buf[wr_buf_p][0]>>(7-i))&1) ? HIGH : LOW;
      wr_buf[i + 40] = ((rgb_buf[wr_buf_p][2]>>(7-i))&1) ? HIGH : LOW;
    }
    wr_buf_p++;
  } else if (wr_buf_p < NUM_PIXELS + 2) {
    for(uint8_t i = 24; i < 48; i++){ wr_buf[i] = 0;};
    wr_buf_p++;
  } else {
    // We're done. Lean back and until next time!
//    wr_buf_p = 0;
    HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_2);
  }
}
*/

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	Neopixel.do_forwardRewrite();

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	Neopixel.do_backRewrite();
}
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
  MX_DMA_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
//  Neopixel.init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Neopixel.set_color(k, 3, 3, 0);
	  Neopixel.show();
//	   Neopixel.set_color(8, 3, 0, 0);
//	       Neopixel.show();
//  set_color(k,1,3,0);
//  show();
  k++;
  if(k>256){
      k=0;
      Neopixel.clear();
//      clear();
    }

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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 9;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 4-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 15-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
