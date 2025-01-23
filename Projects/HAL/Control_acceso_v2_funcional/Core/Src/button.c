#include "button.h"
#include "main.h"

volatile uint8_t button_pressed = 0;
uint32_t b1_tick = 0;

uint8_t button_driver_get_event(void) {
    uint8_t event = button_pressed;
    button_pressed = 0;
    return event;
}

void detect_button_press(void) {
    if (HAL_GetTick() - b1_tick < 50) {
        return; // Ignore bounces less than 50ms
    } else if (HAL_GetTick() - b1_tick > 500) {
        button_pressed = 1; // single press
    } else {
        button_pressed = 2; // double press
    }
    b1_tick = HAL_GetTick();
}