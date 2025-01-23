#include "state_machine.h"
#include "main.h"

#define TEMP_UNLOCK_DURATION 5000

static state_t current_state = LOCKED;
static uint32_t unlock_timer = 0;

extern UART_HandleTypeDef huart2;

void handle_event(uint8_t event) {
    if (event == 1) { // Single press
        HAL_UART_Transmit(&huart2, (uint8_t*)"Single press\r\n", 14, 100);
        HAL_GPIO_WritePin(DOOR_STATUS_GPIO_Port, DOOR_STATUS_Pin, GPIO_PIN_SET);
        current_state = TEMP_UNLOCK;
        unlock_timer = HAL_GetTick();
    } else if (event == 2) { // Double press
        HAL_UART_Transmit(&huart2, (uint8_t*)"Double press\r\n", 14, 100);
        HAL_GPIO_WritePin(DOOR_STATUS_GPIO_Port, DOOR_STATUS_Pin, GPIO_PIN_SET);
        current_state = PERM_UNLOCK;
    } else if (event == 'O') { // Open command
        HAL_UART_Transmit(&huart2, (uint8_t*)"Open command\r\n", 14, 100);
        HAL_GPIO_WritePin(DOOR_STATUS_GPIO_Port, DOOR_STATUS_Pin, GPIO_PIN_SET);
        current_state = TEMP_UNLOCK;
        unlock_timer = HAL_GetTick();
    } else if (event == 'C') { // Close command
        HAL_UART_Transmit(&huart2, (uint8_t*)"Close command\r\n", 15, 100);
        HAL_GPIO_WritePin(DOOR_STATUS_GPIO_Port, DOOR_STATUS_Pin, GPIO_PIN_RESET);
        current_state = LOCKED;
    }
}

void run_state_machine(void) {
    switch (current_state) {
        case LOCKED:
            // No periodic action in locked state
            break;
        case TEMP_UNLOCK:
            if (HAL_GetTick() - unlock_timer >= TEMP_UNLOCK_DURATION) {
                HAL_GPIO_WritePin(DOOR_STATUS_GPIO_Port, DOOR_STATUS_Pin, GPIO_PIN_RESET);
                current_state = LOCKED;
            }
            break;
        case PERM_UNLOCK:
            // No periodic action in permanent unlock state
            break;
    }
}
