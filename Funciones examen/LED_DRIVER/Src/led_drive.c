#include "led_drive.h"
#include "systick.h"
#include "gpio.h"

void start_toggle_task(LedToggleTask *task, uint8_t led_pin) {
    task->led_pin = led_pin;
    task->count = 0;
    task->last_tick = systick_GetTick(COUNTER_B);
    task->active = 1;
}

void handle_toggle_task(LedToggleTask *task, uint32_t current_tick) {
    if (task->active) {
        if (current_tick - task->last_tick >= 500) {
            gpio_toggle_led(task->led_pin);
            task->last_tick = current_tick;
            task->count++;

            if (task->count >= 6) {
                task->active = 0;  // Finaliza la tarea tras 6 toggles (3 ciclos de parpadeo)
            }
        }
    }
}
