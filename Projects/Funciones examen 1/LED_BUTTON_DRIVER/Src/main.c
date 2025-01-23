#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "led_drive.h"

#define LED_IN 5    // Pin 5 de GPIOA
#define LED_R 7    // Pin 7 de GPIOA
#define LED_L 6    // Pin 6 de GPIOA

#define BUTT_L 4 // Pin 2 de GPIOB
#define BUTT_R 5 // Pin 3 de GPIOB

int main(void) {
    // Configurar el systick y los pines GPIO
    configure_systick_and_start();
    configure_gpio(BUTT_L, BUTT_R, LED_L, LED_R, LED_IN);

    LedToggleTask toggle_task = {0}; // Tarea de parpadeo

    while (1) {
        // Heartbeat LED
        if (systick_GetTick(COUNTER_A) >= 500) {   
            gpio_toggle_led(LED_IN);
            systick_reset(COUNTER_A);
        }

        // Verificar botones y activar tarea de parpadeo
        if (!toggle_task.active && gpio_button_flag(BUTT_L)) {
            start_toggle_task(&toggle_task, LED_L);
        }

        if (!toggle_task.active && gpio_button_flag(BUTT_R)) {
            start_toggle_task(&toggle_task, LED_R);
        }

        // Manejar la tarea de parpadeo
        handle_toggle_task(&toggle_task, systick_GetTick(COUNTER_B));
    }
}