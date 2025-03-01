
#include "main.h"

#include "keypad.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
ring_buffer_t rb;
uint8_t buffer_memory[BUFFER_CAPACITY];

uint8_t door_status = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
 */


void handle_UART_interruption(uint8_t *rx_data) {


}



uint8_t rx_data;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // Verificamos que la interrupción provenga de USART2
    if (huart->Instance == USART2)
    {   
    
        handle_UART_interruption(&rx_data);
       
      
      // Reiniciar la recepción para seguir recibiendo datos
        HAL_UART_Receive_IT(&huart2, &rx_data, 1);
    }
    if (huart->Instance == USART3) {

        handle_UART_interruption(&rx_data);
      

        HAL_UART_Receive_IT(&huart3, &rx_data, 1);
    }
}

void heartbeat(void)
{
  static uint32_t last_heartbeat = 0;
  if (HAL_GetTick() - last_heartbeat > 1000)
  {
    last_heartbeat = HAL_GetTick();
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }
}



uint8_t door_status = 0;
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
  keypad_init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart2, (uint8_t *)"Hello, stm32\r\n", 14, HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart3, (uint8_t *)"Hello, esp-wifi\r\n", 17, HAL_MAX_DELAY);
  HAL_UART_Receive_IT(&huart3, &rx_data, 1);
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);
  ring_buffer_init(&rb, buffer_memory, BUFFER_CAPACITY);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint8_t key = keypad_get_pressed_key();
    if (key) {
      ring_buffer_write(&rb, key);
      show_rb(&rb, &huart2);
    }
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
}
  /* USER CODE END 3 */