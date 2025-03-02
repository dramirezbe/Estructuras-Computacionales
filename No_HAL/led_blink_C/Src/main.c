/**
 * @file main.c
 * @brief Simple program to blink an LED using a button press and SysTick timer on STM32.
 */

#include <stdint.h> // Incluye tipos enteros estándar como uint32_t

/// Dirección base del registro RCC
#define RCC_BASE 0x40021000
/// Dirección del registro RCC_AHB2ENR (para habilitar los puertos GPIO)
#define RCC_AHB2ENR ((uint32_t *)(RCC_BASE + 0x4C))

/**
 * @brief Definición de registros del periférico SysTick.
 */
typedef struct {
    volatile uint32_t CTRL;  ///< Control register
    volatile uint32_t LOAD;  ///< Reload value register
    volatile uint32_t VAL;   ///< Current value register
    volatile uint32_t CALIB; ///< Calibration register
} SysTick_t;

/**
 * @brief Definición de registros de un puerto GPIO.
 */
typedef struct {
    volatile uint32_t MODER;    ///< Mode register
    volatile uint32_t OTYPER;   ///< Output type register
    volatile uint32_t OSPEEDR;  ///< Output speed register
    volatile uint32_t PUPDR;    ///< Pull-up/pull-down register
    volatile uint32_t IDR;      ///< Input data register
    volatile uint32_t ODR;      ///< Output data register
    volatile uint32_t BSRR;     ///< Bit set/reset register
    volatile uint32_t LCKR;     ///< Configuration lock register
    volatile uint32_t AFR[2];   ///< Alternate function registers
} GPIO_t;

/// Dirección base del puerto GPIOA
#define GPIOA ((GPIO_t *)0x48000000)
/// Dirección base del puerto GPIOC
#define GPIOC ((GPIO_t *)0x48000800)
/// Dirección base del registro SysTick
#define SysTick ((SysTick_t *)0xE000E010)

/// Pin del LED conectado al GPIOA
#define LED_PIN 5
/// Pin del botón conectado al GPIOC
#define BUTTON_PIN 13

/// Contador global para milisegundos
volatile uint32_t ms_counter = 0;

/**
 * @brief Configura y activa el temporizador SysTick para generar interrupciones cada 1 ms.
 */
void configure_systick_and_start(void) {
    SysTick->CTRL = 0x4;     // Desactiva SysTick para configuración, usa el reloj del procesador
    SysTick->LOAD = 3999;    // Valor de recarga para 1 ms (asumiendo un reloj de 4 MHz)
    SysTick->CTRL = 0x5;     // Habilita SysTick con el reloj del procesador, sin interrupciones
}

/**
 * @brief Inicializa un pin GPIO con el modo especificado.
 * @param GPIOx Puntero al periférico GPIO (GPIOA, GPIOB, etc.)
 * @param pin Número del pin a configurar.
 * @param mode Modo del pin (0x0 = entrada, 0x1 = salida, etc.).
 */
void init_gpio_pin(GPIO_t *GPIOx, uint8_t pin, uint8_t mode) {
    GPIOx->MODER &= ~(0x3 << (pin * 2)); // Limpia los bits MODER para este pin
    GPIOx->MODER |= (mode << (pin * 2)); // Establece los bits MODER para este pin
}

/**
 * @brief Configura los pines GPIO para el LED y el botón.
 */
void configure_gpio(void) {
    *RCC_AHB2ENR |= (1 << 0) | (1 << 2); // Habilita el reloj para GPIOA y GPIOC

    init_gpio_pin(GPIOA, LED_PIN, 0x1);  // Configura el pin del LED como salida
    init_gpio_pin(GPIOC, BUTTON_PIN, 0x0); // Configura el pin del botón como entrada
}

/**
 * @brief Función principal del programa.
 * @return Nunca retorna (bucle infinito).
 */
int main(void) {
    ms_counter = 0; // Inicializa el contador a 0

    configure_systick_and_start(); // Configura y activa SysTick
    configure_gpio(); // Configura los pines GPIO

    uint8_t state = 0; // Estado inicial de la máquina de estados

    while (1) { // Bucle principal

        switch (state) {
        case 0: // Estado IDLE
            if (!(GPIOC->IDR & (1 << BUTTON_PIN))) { // Si el botón está presionado
                state = 1; // Cambia al estado BUTTON_PRESSED
            }
            else if (ms_counter >= 500) { // Si han pasado 500 ms
                state = 2;
            }
            break;

        case 1: // Estado BUTTON_PRESSED
            if ((GPIOC->IDR & (1 << BUTTON_PIN))) { // Si el botón está presionado
                ms_counter = 0;
                state = 0; // Cambia al estado BUTTON_PRESSED
                    
            }
            
            break;

        case 2: // Estado TOGGLE_LED

            GPIOA->ODR ^= (1 << LED_PIN); // Cambia el estado del LED
            ms_counter = 0;
            state = 0; // Vuelve al estado IDLE
    
            break;

        default:
            break;
        }

        if (SysTick->CTRL & (1 << 16)) { // Verifica si se ha establecido el flag COUNTFLAG
            ms_counter++; // Incrementa el contador de milisegundos
        }
    }
}