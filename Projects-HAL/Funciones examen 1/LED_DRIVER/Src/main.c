<<<<<<< HEAD
#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "led_drive.h"

#define LED_PIN_IN 5    // Pin 5 de GPIOA
#define LED_PIN_RIGHT 7    // Pin 4 de GPIOA
#define LED_PIN_LEFT 6    // Pin 6 de GPIOA

#define BUTTON_PIN_1 13 // Pin 13 de GPIOC
#define BUTTON_PIN_2 14 // Pin 14 de GPIOC

int main(void) {
    // Configurar el systick y los pines GPIO
    configure_systick_and_start();
    configure_gpio(BUTTON_PIN_1, BUTTON_PIN_2, LED_PIN_RIGHT, LED_PIN_LEFT, LED_PIN_IN);

    LedToggleTask toggle_task = {0}; // Tarea de parpadeo

    while (1) {
        // Heartbeat LED
        if (systick_GetTick(COUNTER_A) >= 500) {   
            gpio_toggle_led(LED_PIN_IN);
            systick_reset(COUNTER_A);
        }

        // Verificar botones y activar tarea de parpadeo
        if (!toggle_task.active && gpio_button_flag(BUTTON_PIN_1)) {
            start_toggle_task(&toggle_task, LED_PIN_RIGHT);
        }

        if (!toggle_task.active && gpio_button_flag(BUTTON_PIN_2)) {
            start_toggle_task(&toggle_task, LED_PIN_LEFT);
        }

        // Manejar la tarea de parpadeo
        handle_toggle_task(&toggle_task, systick_GetTick(COUNTER_B));
    }
=======
#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "led_drive.h"

#define LED_PIN_IN 5    // Pin 5 de GPIOA
#define LED_PIN_RIGHT 7    // Pin 4 de GPIOA
#define LED_PIN_LEFT 6    // Pin 6 de GPIOA

#define BUTTON_PIN_1 13 // Pin 13 de GPIOC
#define BUTTON_PIN_2 14 // Pin 14 de GPIOC

int main(void) {
    // Configurar el systick y los pines GPIO
    configure_systick_and_start();
    configure_gpio(BUTTON_PIN_1, BUTTON_PIN_2, LED_PIN_RIGHT, LED_PIN_LEFT, LED_PIN_IN);

    LedToggleTask toggle_task = {0}; // Tarea de parpadeo

    while (1) {
        // Heartbeat LED
        if (systick_GetTick(COUNTER_A) >= 500) {   
            gpio_toggle_led(LED_PIN_IN);
            systick_reset(COUNTER_A);
        }

        // Verificar botones y activar tarea de parpadeo
        if (!toggle_task.active && gpio_button_flag(BUTTON_PIN_1)) {
            start_toggle_task(&toggle_task, LED_PIN_RIGHT);
        }

        if (!toggle_task.active && gpio_button_flag(BUTTON_PIN_2)) {
            start_toggle_task(&toggle_task, LED_PIN_LEFT);
        }

        // Manejar la tarea de parpadeo
        handle_toggle_task(&toggle_task, systick_GetTick(COUNTER_B));
    }
>>>>>>> af4b251ca32924dfacd5b62a67d5a1a332606774
}