/**
 ******************************************************************************
 * @file           : main.c
 * @author         : David Ramírez Betancourth
 * @brief          : Control de LED con botón utilizando interrupciones.
 ******************************************************************************
 * @attention
 *
 * Este código configura un LED para parpadear a intervalos regulares,
 * y utiliza un botón conectado a un pin configurado como interrupción
 * externa para alternar entre congelar y descongelar el parpadeo.
 *
 ******************************************************************************
 */

#include <stdint.h>

/**
 * @brief Estructura que representa los registros del módulo SYSCFG.
 */
typedef struct {
    volatile uint32_t MEMRMP;   /*!< Memory remap register */
    volatile uint32_t CFGR1;    /*!< Configuration register 1 */
    volatile uint32_t EXTICR[4];/*!< External interrupt configuration registers */
    volatile uint32_t CFGR2;    /*!< Configuration register 2 */
} SYSCFG_t;

/**
 * @brief Estructura que representa los registros del módulo EXTI.
 */
typedef struct {
    volatile uint32_t IMR1;     /*!< Interrupt mask register 1 */
    volatile uint32_t EMR1;     /*!< Event mask register 1 */
    volatile uint32_t RTSR1;    /*!< Rising trigger selection register 1 */
    volatile uint32_t FTSR1;    /*!< Falling trigger selection register 1 */
    volatile uint32_t SWIER1;   /*!< Software interrupt event register 1 */
    volatile uint32_t PR1;      /*!< Pending register 1 */
    volatile uint32_t IMR2;     /*!< Interrupt mask register 2 */
    volatile uint32_t EMR2;     /*!< Event mask register 2 */
    volatile uint32_t RTSR2;    /*!< Rising trigger selection register 2 */
    volatile uint32_t FTSR2;    /*!< Falling trigger selection register 2 */
    volatile uint32_t SWIER2;   /*!< Software interrupt event register 2 */
    volatile uint32_t PR2;      /*!< Pending register 2 */
} EXTI_t;

/**
 * @brief Estructura que representa los registros del módulo SysTick.
 */
typedef struct {
    volatile uint32_t CTRL;     /*!< Control and status register */
    volatile uint32_t LOAD;     /*!< Reload value register */
    volatile uint32_t VAL;      /*!< Current value register */
    volatile uint32_t CALIB;    /*!< Calibration value register */
} SysTick_t;

/**
 * @brief Estructura que representa los registros del módulo GPIO.
 */
typedef struct {
    volatile uint32_t MODER;    /*!< Mode register */
    volatile uint32_t OTYPER;   /*!< Output type register */
    volatile uint32_t OSPEEDR;  /*!< Output speed register */
    volatile uint32_t PUPDR;    /*!< Pull-up/pull-down register */
    volatile uint32_t IDR;      /*!< Input data register */
    volatile uint32_t ODR;      /*!< Output data register */
    volatile uint32_t BSRR;     /*!< Bit set/reset register */
    volatile uint32_t LCKR;     /*!< Configuration lock register */
    volatile uint32_t AFR[2];   /*!< Alternate function registers */
    volatile uint32_t BRR;      /*!< Bit reset register */
} GPIO_t;

/* Definiciones de direcciones base de periféricos */
#define EXTI_BASE 0x40010400
#define EXTI ((EXTI_t *)EXTI_BASE)

#define SYSCFG_BASE 0x40010000
#define SYSCFG ((SYSCFG_t *)SYSCFG_BASE)

#define GPIOA ((GPIO_t *)0x48000000) // Base address of GPIOA
#define GPIOC ((GPIO_t *)0x48000800) // Base address of GPIOC

#define RCC_BASE 0x40021000
#define RCC_AHB2ENR ((uint32_t *)(RCC_BASE + 0x4C)) /*!< AHB2 peripheral clock enable register */
#define RCC_APB2ENR ((uint32_t *)(RCC_BASE + 0x60)) /*!< APB2 peripheral clock enable register */

#define SysTick ((SysTick_t *)0xE000E010) // Base address of SysTick

/* Definiciones relacionadas con pines */
#define LED_PIN 5              /*!< Pin del LED (PA5) */
#define BUTTON_PIN 13          /*!< Pin del botón (PC13) */

#define NVIC_ISER1 ((uint32_t *)(0xE000E104)) /*!< NVIC Interrupt Set-Enable Register */

#define EXTI15_10_IRQn 40 /*!< IRQ para EXTI líneas 10-15 */

/* Macros para manejo del botón y el LED */
#define BUTTON_IS_PRESSED()    (!(GPIOC->IDR & (1 << BUTTON_PIN)))
#define BUTTON_IS_RELEASED()   (GPIOC->IDR & (1 << BUTTON_PIN))
#define TOGGLE_LED()           (GPIOA->ODR ^= (1 << LED_PIN))

/* Variables globales */
volatile uint32_t ms_counter = 0; /*!< Contador de milisegundos */
volatile uint8_t freeze_led = 0;  /*!< Estado de congelación del LED */

/**
 * @brief Configura y habilita el temporizador SysTick para generar interrupciones cada 1 ms.
 */
void configure_systick_and_start(void)
{
    SysTick->CTRL = 0x4;     // Deshabilitar SysTick para configuración
    SysTick->LOAD = 3999;    // Recarga para 1 ms (asumiendo reloj de 4 MHz)
    SysTick->CTRL = 0x7;     // Habilitar SysTick, reloj del procesador y generación de interrupciones
}

/**
 * @brief Inicializa un pin GPIO en el modo especificado.
 * @param GPIOx Puntero a la estructura GPIO.
 * @param pin Número del pin a configurar.
 * @param mode Modo de operación (0x0 = entrada, 0x1 = salida, etc.).
 */
void init_gpio_pin(GPIO_t *GPIOx, uint8_t pin, uint8_t mode)
{
    GPIOx->MODER &= ~(0x3 << (pin * 2)); // Limpiar los bits de MODER para el pin
    GPIOx->MODER |= (mode << (pin * 2)); // Configurar el modo deseado
}

/**
 * @brief Configura los periféricos GPIO y EXTI necesarios.
 */
void configure_gpio(void)
{
    *RCC_AHB2ENR |= (1 << 0) | (1 << 2); // Habilitar reloj para GPIOA y GPIOC
    *RCC_APB2ENR |= (1 << 0);            // Habilitar reloj para SYSCFG

    // Configurar EXTI para PC13
    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |= (0x2 << 4);     // Mapear EXTI13 a Port C

    // Configurar EXTI para ambos flancos
    EXTI->FTSR1 |= (1 << BUTTON_PIN);  // Habilitar flanco de bajada
    EXTI->RTSR1 |= (1 << BUTTON_PIN);  // Habilitar flanco de subida
    EXTI->IMR1 |= (1 << BUTTON_PIN);   // Habilitar máscara de interrupción

    // Configurar GPIO
    init_gpio_pin(GPIOA, LED_PIN, 0x1); // Configurar PA5 como salida
    init_gpio_pin(GPIOC, BUTTON_PIN, 0x0); // Configurar PC13 como entrada

    // Habilitar interrupción en NVIC
    *NVIC_ISER1 |= (1 << (EXTI15_10_IRQn - 32));
}

/**
 * @brief Punto de entrada principal.
 * @return No retorna.
 */
int main(void)
{
    configure_systick_and_start();
    configure_gpio();

    while (1) {
        if (!freeze_led && ms_counter >= 500) { // Parpadeo del LED
            TOGGLE_LED();
            ms_counter = 0;
        }
    }
}

/**
 * @brief Manejador de interrupción para SysTick.
 */
void SysTick_Handler(void)
{
    ms_counter++;
}

/**
 * @brief Manejador de interrupción para EXTI líneas 10-15.
 */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR1 & (1 << BUTTON_PIN)) { // Verificar si la interrupción es por el botón
        if (BUTTON_IS_PRESSED()) {
            freeze_led = !freeze_led; // Alternar estado de congelación
        }
        EXTI->PR1 |= (1 << BUTTON_PIN); // Limpiar bandera de interrupción
    }
}
