#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "main.h"

#define ROWS 4
#define COLS 4

// Mapa de teclado mejorado con comentarios
extern const uint8_t keypad_map[ROWS][COLS];

void keypad_init(void);
uint8_t keypad_scan(uint16_t GPIO_Pin);

#endif