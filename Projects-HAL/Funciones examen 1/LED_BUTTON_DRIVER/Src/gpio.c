#include "gpio.h"
#include "rcc.h"


volatile uint8_t button_pressed = 0; // Flag to indicate button press

void configure_gpio_for_usart() {
    // Enable GPIOA clock
    *RCC_AHB2ENR |= (1 << 0);

    // Configure PA2 (TX) as alternate function
    GPIOA->MODER &= ~(3U << (2 * 2)); // Clear mode bits for PA2
    GPIOA->MODER |= (2U << (2 * 2));  // Set alternate function mode for PA2

    // Configure PA3 (RX) as alternate function
    GPIOA->MODER &= ~(3U << (3 * 2)); // Clear mode bits for PA3
    GPIOA->MODER |= (2U << (3 * 2));  // Set alternate function mode for PA3

    // Set alternate function to AF7 for PA2 and PA3
    GPIOA->AFR[0] &= ~(0xF << (4 * 2)); // Clear AFR bits for PA2
    GPIOA->AFR[0] |= (7U << (4 * 2));   // Set AFR to AF7 for PA2
    GPIOA->AFR[0] &= ~(0xF << (4 * 3)); // Clear AFR bits for PA3
    GPIOA->AFR[0] |= (7U << (4 * 3));   // Set AFR to AF7 for PA3

    // Configure PA2 and PA3 as very high speed
    GPIOA->OSPEEDR |= (3U << (2 * 2)); // Very high speed for PA2
    GPIOA->OSPEEDR |= (3U << (3 * 2)); // Very high speed for PA3

    // Configure PA2 and PA3 as no pull-up, no pull-down
    GPIOA->PUPDR &= ~(3U << (2 * 2)); // No pull-up, no pull-down for PA2
    GPIOA->PUPDR &= ~(3U << (3 * 2)); // No pull-up, no pull-down for PA3


    // Configurar PB4 y PB5 como salida (modo 01 en MODER)
    GPIOB->MODER &= ~((3U << (4 * 2)) | (3U << (5 * 2))); // Limpiar bits
    GPIOB->MODER |= ((1U << (4 * 2)) | (1U << (5 * 2)));  // Establecer modo salida

    // Establecer PB4 y PB5 en alto (ODR)
    GPIOB_ODR |= (1U << 4) | (1U << 5);

}

void init_gpio_pin(GPIO_t *GPIOx, uint8_t pin, uint8_t mode)
{
    GPIOx->MODER &= ~(0x3 << (pin * 2)); // Clear MODER bits for this pin
    GPIOx->MODER |= (mode << (pin * 2)); // Set MODER bits for this pin
}



void configure_gpio(uint8_t butt_l, uint8_t butt_r, uint8_t led_l, uint8_t led_r, uint8_t led_in)
{
    // Habilitar el reloj para GPIOA y GPIOC
    *RCC_AHB2ENR |= (1 << 0) | (1 << 2);
    RCC_AHB1ENR |= (1U << 1);

    // Configurar pines de botones como entrada (mode = 0)
    init_gpio_pin(GPIOB, butt_l, 0x0);
    init_gpio_pin(GPIOB, butt_r, 0x0);

    // Configurar pines de LEDs como salida (mode = 1)
    init_gpio_pin(GPIOA, led_l, 0x1);
    init_gpio_pin(GPIOA, led_r, 0x1);
    init_gpio_pin(GPIOA, led_in, 0x1);

    // Configurar SYSCFG para los botones
    *RCC_APB2ENR |= (1 << 0); // Habilitar el reloj para SYSCFG

    // Mapear los pines de botones al controlador EXTI
    SYSCFG->EXTICR[butt_l / 4] &= ~(0xF << (4 * (butt_l % 4)));
    SYSCFG->EXTICR[butt_l / 4] |= (0x2 << (4 * (butt_l % 4))); // Mapear a GPIOC

    SYSCFG->EXTICR[butt_r / 4] &= ~(0xF << (4 * (butt_r % 4)));
    SYSCFG->EXTICR[butt_r / 4] |= (0x2 << (4 * (butt_r % 4))); // Mapear a GPIOC


    // Configurar EXTI para disparos por flanco de bajada
    EXTI->FTSR1 |= (1 << butt_l);
    EXTI->FTSR1 |= (1 << butt_r);

    // Deshabilitar los disparos por flanco de subida
    EXTI->RTSR1 &= ~(1 << butt_l);
    EXTI->RTSR1 &= ~(1 << butt_r);

    // Habilitar las máscaras de interrupción para los botones
    EXTI->IMR1 |= (1 << butt_l);
    EXTI->IMR1 |= (1 << butt_r);

    // Habilitar la interrupción EXTI15_10 en el NVIC
    *NVIC_ISER1 |= (1 << (EXTI15_10_IRQn - 32));
}

uint8_t gpio_button_is_pressed(uint8_t button_pin)
{
    return !(GPIOC->IDR & (1 << button_pin)); // Botón presionado si el bit correspondiente es 0
}

uint8_t gpio_button_is_released(uint8_t button_pin)
{
    return (GPIOC->IDR & (1 << button_pin)); // Botón liberado si el bit correspondiente es 1
}


void gpio_toggle_led(uint8_t led_pin)
{
    GPIOA->ODR ^= (1 << led_pin); // Cambiar el estado del LED
}

volatile uint8_t button_flags[16] = {0}; // Bandera para cada pin de interrupción

void EXTI15_10_IRQHandler(void)
{
    for (uint8_t pin = 10; pin <= 15; pin++) {
        if (EXTI->PR1 & (1 << pin)) {
            EXTI->PR1 = (1 << pin);       // Limpiar el bit pendiente
            button_flags[pin] = 1;        // Establecer la bandera correspondiente
        }
    }
}

uint8_t gpio_button_flag(uint8_t button_pin)
{
    uint8_t flag = button_flags[button_pin];
    button_flags[button_pin] = 0; // Limpiar la bandera después de leer
    return flag;
}