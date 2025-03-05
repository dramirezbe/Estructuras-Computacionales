#include "esp_wifi_handler.h"

void echo_uart(UART_HandleTypeDef huart, char msg) {
    HAL_UART_Transmit(&huart, (uint8_t *)&msg, 1, 1000);
}