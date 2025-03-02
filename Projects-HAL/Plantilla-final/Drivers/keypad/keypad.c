#include "keypad.h"

const uint8_t keypad_map[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Configuración inicial de pines
void keypad_init(void) {
  // Todas las filas como salidas en bajo
  HAL_GPIO_WritePin(ROW_1_GPIO_Port, ROW_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ROW_2_GPIO_Port, ROW_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ROW_3_GPIO_Port, ROW_3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ROW_4_GPIO_Port, ROW_4_Pin, GPIO_PIN_RESET);
}

// Función mejorada para escanear filas
static uint8_t keypad_scan_column(GPIO_TypeDef* column_port, uint16_t column_pin) {
  // Verificar si la columna está inactiva
  if(HAL_GPIO_ReadPin(column_port, column_pin) == GPIO_PIN_SET) {
    return 0;
  }

  // Estructura para manejar filas de forma dinámica
  const struct {
    GPIO_TypeDef* port;
    uint16_t pin;
  } rows[] = {
    {ROW_1_GPIO_Port, ROW_1_Pin},
    {ROW_2_GPIO_Port, ROW_2_Pin},
    {ROW_3_GPIO_Port, ROW_3_Pin},
    {ROW_4_GPIO_Port, ROW_4_Pin}
  };

  // Escanear cada fila individualmente
  for(uint8_t i = 0; i < ROWS; i++) {
    // Activar solo la fila actual
    HAL_GPIO_WritePin(rows[i].port, rows[i].pin, GPIO_PIN_SET);
    
    // Delay breve y estable (40 ciclos)
    for(volatile int j = 0; j < 40; j++);

    // Detectar cambio en la columna
    if(HAL_GPIO_ReadPin(column_port, column_pin) == GPIO_PIN_SET) {
      // Desactivar fila antes de retornar
      HAL_GPIO_WritePin(rows[i].port, rows[i].pin, GPIO_PIN_RESET);
      return i + 1; // Retornar número de fila (1-4)
    }
    
    // Desactivar fila
    HAL_GPIO_WritePin(rows[i].port, rows[i].pin, GPIO_PIN_RESET);
  }
  
  return 0;
}

// Función principal de escaneo mejorada
uint8_t keypad_scan(uint16_t GPIO_Pin) {
  uint8_t row = 0;
  uint8_t col = 0;
  uint8_t key = 'E'; // Carácter por defecto para error

  // Determinar columna presionada
  switch(GPIO_Pin) {
    case COLUMN_1_Pin: col = 0; break;
    case COLUMN_2_Pin: col = 1; break;
    case COLUMN_3_Pin: col = 2; break;
    case COLUMN_4_Pin: col = 3; break;
    default: return key;
  }

  // Obtener fila activa
  GPIO_TypeDef* port = NULL;
  switch(GPIO_Pin) {
    case COLUMN_1_Pin: port = COLUMN_1_GPIO_Port; break;
    case COLUMN_2_Pin: port = COLUMN_2_GPIO_Port; break;
    case COLUMN_3_Pin: port = COLUMN_3_GPIO_Port; break;
    case COLUMN_4_Pin: port = COLUMN_4_GPIO_Port; break;
  }
  
  row = keypad_scan_column(port, GPIO_Pin);

  // Mapear a tecla solo si se detectó una fila válida
  if(row > 0 && row <= ROWS) {
    key = keypad_map[row - 1][col];
  }

  // Resetear estado del teclado
  keypad_init();
  
  return key;
}