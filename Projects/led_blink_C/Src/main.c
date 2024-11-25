#include <stdint.h>

#define STK_BASE 0xE000E010
#define STK_CTRL (*(volatile uint32_t *)(STK_BASE + 0x00))
#define STK_LOAD (*(volatile uint32_t *)(STK_BASE + 0x04))

#define RCC_BASE 0x40021000
#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x4C))

#define GPIOA_BASE 0x48000000
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define LED_PIN 5

#define GPIOC_BASE 0x48000800
#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_IDR (*(volatile uint32_t *)(GPIOC_BASE + 0x10))
#define BUTTON_PIN 13

volatile uint32_t ms_counter = 0;

void init_ms_counter(void) {
    ms_counter = 0;
}

void configure_systick_and_start(void) {
    STK_CTRL = 0x4; // Disable SysTick for configuration, use processor clock
    STK_LOAD = 3999; // Reload value for 1 ms (assuming 4 MHz clock)
    STK_CTRL = 0x5; // Enable SysTick, processor clock, no interrupt
}

void configure_gpio(void) {
    RCC_AHB2ENR |= (1 << 0) | (1 << 2); // Enable clock for GPIOA and GPIOC

    // Configure GPIOA pin 5 as output (01)
    GPIOA_MODER &= ~(0x3 << (LED_PIN * 2));
    GPIOA_MODER |= (0x1 << (LED_PIN * 2));

    // Configure GPIOC pin 13 as input (00)
    GPIOC_MODER &= ~(0x3 << (BUTTON_PIN * 2));
}

void check_button_and_blink(void) {
    ms_counter++;

    if (!(GPIOC_IDR & (1 << BUTTON_PIN))) { // If button is pressed
        ms_counter = 0;
    }

    if (ms_counter >= 500) { // Blink LED every 500 ms
        GPIOA_ODR ^= (1 << LED_PIN); // Toggle LED
        ms_counter = 0; // Reset counter after blinking
    }
}

int main(void) {
    init_ms_counter();
    configure_systick_and_start();
    configure_gpio();

    while (1) {
        if (STK_CTRL & (1 << 16)) { // Check if COUNTFLAG is set
            check_button_and_blink();
        }
    }
}