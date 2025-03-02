#ifndef DIMMER_H
#define DIMMER_H

#include "main.h"

// Inicializa variables internas del módulo dimmer (si se desea)
void Dimmer_Init(void);

// Funciones que deben llamarse desde los callbacks HAL
void Dimmer_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Dimmer_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Dimmer_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* DIMMER_H */
