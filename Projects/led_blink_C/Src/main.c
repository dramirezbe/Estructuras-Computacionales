#include <stdint.h>

#define STK_BASE       0xE000E010
#define STK_CTRL       (*(volatile uint32_t *)(STK_BASE + 0x00))
#define STK_LOAD       (*(volatile uint32_t *)(STK_BASE + 0x04))

#define RCC_BASE       0x40021000
#define RCC_AHB2ENR    (*(volatile uint32_t *)(RCC_BASE + 0x4C))

#define GPIOA_BASE     0x48000000
#define GPIOA_MODER    (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR      (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

#define GPIOC_BASE     0x48000800
#define GPIOC_MODER    (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_IDR      (*(volatile uint32_t *)(GPIOC_BASE + 0x10))

#define LED_PIN        5
#define BUTTON_PIN     13

volatile uint32_t ms_counter = 0;

void init_ms_counter(void) {
    ms_counter = 0;
}

void configure_systick_and_start(void) {
    STK_CTRL = 0x4;       // Deshabilitar SysTick
    STK_LOAD = 3999;      // Configurar valor de recarga para 1 ms (asumiendo 4 MHz)
    STK_CTRL = 0x5;       // Habilitar SysTick sin interrupciones
}

void configure_gpio(void) {
    // Habilitar reloj para GPIOA y GPIOC
    RCC_AHB2ENR |= (1 << 0) | (1 << 2);

    // Configurar LED_PIN (PA5) como salida
    GPIOA_MODER &= ~(0x3 << (LED_PIN * 2)); // Limpiar bits
    GPIOA_MODER |= (0x1 << (LED_PIN * 2));  // Configurar como salida

    // Configurar BUTTON_PIN (PC13) como entrada
    GPIOC_MODER &= ~(0x3 << (BUTTON_PIN * 2)); // Configurar como entrada
}

void increase_ms_counter_and_blink(void) {
    ms_counter++;

    if (ms_counter >= 500) {
        // Leer estado del botón
        if ((GPIOC_IDR & (1 << BUTTON_PIN)) == 0) {
            ms_counter = 0; // Reiniciar contador si el botón está presionado
        } else {
            GPIOA_ODR ^= (1 << LED_PIN); // Alternar estado del LED
            ms_counter = 0;             // Reiniciar contador
        }
    }
}

int main(void) {
    init_ms_counter();
    configure_systick_and_start();
    configure_gpio();

    while (1) {
        if (STK_CTRL & (1 << 16)) { // Verificar COUNTFLAG
            increase_ms_counter_and_blink();
        }
    }

    return 0;
}
