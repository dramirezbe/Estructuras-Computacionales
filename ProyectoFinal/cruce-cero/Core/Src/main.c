#include "main.h"

/* Variables globales */
volatile uint8_t zero_detected = 0; // Bandera que se activa en el cruce por cero
volatile uint8_t dim = 0;           // Valor de atenuación (0 a 10)

UART_HandleTypeDef huart2;

#define HALF_CYCLE_US 8333  // Medio ciclo en microsegundos para 60 Hz

/* Buffer para recepción por USART2 */
#define RX_BUFFER_SIZE 10
uint8_t uartRxBuffer[RX_BUFFER_SIZE];
uint8_t rxIndex = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* Prototipos de funciones */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void delay_us(uint32_t us);
int map(int x, int in_min, int in_max, int out_min, int out_max);


int main(void)
{
  

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /* Iniciar la recepción UART en modo interrupción */
  HAL_UART_Receive_IT(&huart2, &uartRxBuffer[rxIndex], 1);

  HAL_UART_Transmit(&huart2, (uint8_t*)"UART2-init\r\n", 14, 1000);

  uint32_t lastTick = HAL_GetTick(); 

  while (1)
  {
    if (zero_detected)
    {
      /* Apaga el TRIAC inicialmente (PB1) */
      HAL_GPIO_WritePin(TRIAC_PULSE_GPIO_Port, TRIAC_PULSE_Pin, GPIO_PIN_RESET);

      if (dim == 0)
      {
        /* Sin retardo: dispara el TRIAC inmediatamente */
        HAL_GPIO_WritePin(TRIAC_PULSE_GPIO_Port, TRIAC_PULSE_Pin, GPIO_PIN_SET);
      }
      else
      {
        /* Calcula el retardo (en microsegundos) mapeando el valor recibido */
        int retardo = map(dim, 1, 10, 100, HALF_CYCLE_US - 100);
        delay_us(retardo);
        HAL_GPIO_WritePin(TRIAC_PULSE_GPIO_Port, TRIAC_PULSE_Pin, GPIO_PIN_SET);
      }
      /* Pulso corto para disparar el TRIAC */
      delay_us(10);
      HAL_GPIO_WritePin(TRIAC_PULSE_GPIO_Port, TRIAC_PULSE_Pin, GPIO_PIN_RESET);

      /* Reinicia la bandera de cruce por cero */
      zero_detected = 0;
    }
    /* Envía el valor 'dim' cada 500 ms utilizando HAL_GetTick (basado en SysTick) */
    if ((HAL_GetTick() - lastTick) >= 500)
    {
      lastTick = HAL_GetTick();
      char dimString[4];  // Para valores entre 0 y 10, 3 dígitos máximo + '\0'
      sprintf(dimString, "%d", dim);
      HAL_UART_Transmit(&huart2, (uint8_t*)dimString, strlen(dimString), 1000);
      HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 1000);  // Envía salto de línea
    }
  }
}

/* Callback de la interrupción EXTI: se llama al detectar un flanco en PB2 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == ZERO_DETECT_Pin)  // PB2 configurado como ZERO_DETECT
  {
    zero_detected = 1;
  }
}

/* Callback de recepción UART: procesa el número recibido (0-10) */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2)
  {
    // Transmitir el byte recibido para depuración (echo)
    HAL_UART_Transmit(&huart2, &uartRxBuffer[rxIndex], 1, 100);

    // Si se detecta fin de mensaje ('\n' o '\r'), procesamos el número
    if(uartRxBuffer[rxIndex] == '\n' || uartRxBuffer[rxIndex] == '\r')
    {
      uartRxBuffer[rxIndex] = '\0'; // Finaliza la cadena
      int valor = atoi((char*)uartRxBuffer);
      if(valor >= 0 && valor <= 10)
      {
        dim = (uint8_t)valor;
      }
      rxIndex = 0; // Reinicia el índice para la próxima recepción
    }
    else
    {
      rxIndex++;
      if(rxIndex >= RX_BUFFER_SIZE)
      {
        rxIndex = 0; // Reinicia en caso de desbordamiento del buffer
      }
    }
    // Reinicia la recepción de un byte
    HAL_UART_Receive_IT(&huart2, &uartRxBuffer[rxIndex], 1);
  }
}


/* Función para generar un retardo en microsegundos utilizando el contador DWT */
void delay_us(uint32_t us)
{
  uint32_t startTick = DWT->CYCCNT;
  uint32_t ticks = us * (SystemCoreClock / 1000000);
  while((DWT->CYCCNT - startTick) < ticks);
}

/* Función para mapear un valor de un rango a otro */
int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}







void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TRIAC_PULSE_GPIO_Port, TRIAC_PULSE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TRIAC_PULSE_Pin */
  GPIO_InitStruct.Pin = TRIAC_PULSE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(TRIAC_PULSE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ZERO_DETECT_Pin */
  GPIO_InitStruct.Pin = ZERO_DETECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ZERO_DETECT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

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
