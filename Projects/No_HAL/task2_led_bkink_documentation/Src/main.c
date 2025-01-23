/**
 ******************************************************************************
 * @file           : main.c
 * @author         : David Ramírez Betanocurth
 * @brief          : Led blink documentation program.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>

// Base de SysTick y registros
#define SYSTICK_BASE 0xE000E010U
#define SYSTICK_CTRL (*(volatile uint32_t *)(SYSTICK_BASE + 0x0))
#define SYSTICK_LOAD (*(volatile uint32_t *)(SYSTICK_BASE + 0x4))
#define SYSTICK_VAL (*(volatile uint32_t *)(SYSTICK_BASE + 0x8))
#define SYSTICK_CALIB (*(volatile uint32_t *)(SYSTICK_BASE + 0xC))

// Bits de control en el registro SysTick_CTRL
#define SYSTICK_CTRL_ENABLE (1U << 0)  // Bit de habilitación
#define SYSTICK_CTRL_TICKINT (1U << 1) // Bit de interrupción
#define SYSTICK_CTRL_CLKSRC (1U << 2)  // Fuente de reloj (1: procesador)

// Prototipos de funciones
void SysTick_Init(uint32_t ticks);
void SysTick_DelayMs(uint32_t delay_ms);
void SysTick_Handler(void); // Manejador de interrupciones (debe enlazarse con el vector)

uint32_t SysTick_GetTick(void);

// Habilita interrupciones globales
void EnableGlobalInterrupts(void)
{
    __asm volatile("CPSIE i" ::: "memory");
}

// Macros para RCC
#define RCC_BASE 0x40021000U
#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x60))

// Macros para habilitar periféricos
#define RCC_GPIOA_ENABLE (1U << 0)
#define RCC_GPIOC_ENABLE (1U << 2)

// Prototipos de funciones
void RCC_EnableAHB2(uint32_t peripherals);
void RCC_EnableAPB2(uint32_t peripherals);

// Base addresses para GPIO
#define GPIOA_BASE 0x48000000U
#define GPIOC_BASE 0x48000800U

// Registros GPIO
#define GPIO_MODER_OFFSET 0x00U
#define GPIO_ODR_OFFSET 0x14U

#define GPIO_MODER(gpio) (*(volatile uint32_t *)((gpio) + GPIO_MODER_OFFSET))
#define GPIO_ODR(gpio) (*(volatile uint32_t *)((gpio) + GPIO_ODR_OFFSET))

// Prototipos de funciones
void GPIO_SetPinAsOutput(uint32_t gpio_base, uint32_t pin);
void GPIO_SetPin(uint32_t gpio_base, uint32_t pin);
void GPIO_ClearPin(uint32_t gpio_base, uint32_t pin);
void GPIO_TogglePin(uint32_t gpio_base, uint32_t pin);

// Variable estática para almacenar los ticks del sistema
static volatile uint32_t system_ticks = 0;

int main(void)
{
    // Habilitar relojes
    RCC_EnableAPB2(0x1); // Ejemplo: habilitar periféricos de APB2
    
    // Iniciliza el reloj del sistema 1ms para clk = 4MHz
    SysTick_Init(3999);

    // Habilita interrupciones globales
    EnableGlobalInterrupts();
    
    // Habilita el reloj de los puertos necesarios
    RCC_EnableAHB2(RCC_GPIOA_ENABLE | RCC_GPIOC_ENABLE);

    // Configurar GPIOA (pin 5 como salida)
    GPIO_SetPinAsOutput(GPIOA_BASE, 5);

    GPIO_SetPin(GPIOA_BASE, 5);   // Encender LED
    GPIO_ClearPin(GPIOA_BASE, 5); // Apagar LED

    uint32_t last_tick = 0;
    // Bucle infinito
    while (1)
    {
        if ((SysTick_GetTick() - last_tick) >= 500)
        {
            last_tick = SysTick_GetTick();
            GPIO_TogglePin(GPIOA_BASE, 5);   // Cambiar de estado LED
        }
    }

    return 0;
}

/**
 * @brief Inicializa el temporizador SysTick.
 * @param ticks: Número de ticks antes de que ocurra la interrupción.
 *               Este valor se usa para generar un intervalo de tiempo fijo.
 */
void SysTick_Init(uint32_t ticks)
{
    // Configurar el valor de recarga
    SYSTICK_LOAD = ticks - 1;

    // Reiniciar el valor del contador actual
    SYSTICK_VAL = 0;

    // Configurar y habilitar SysTick con interrupciones
    SYSTICK_CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_TICKINT | SYSTICK_CTRL_CLKSRC;
}

/**
 * @brief Genera un retardo en milisegundos utilizando el contador SysTick.
 * @param delay_ms: Tiempo en milisegundos a esperar.
 */
void SysTick_DelayMs(uint32_t delay_ms)
{
    uint32_t start_tick = SysTick_GetTick();
    while ((SysTick_GetTick() - start_tick) < delay_ms)
    {
        // Esperar el tiempo especificado
    }
}

/**
 * @brief Devuelve el número de ticks desde el inicio del sistema.
 * @return Número de ticks del sistema.
 */
uint32_t SysTick_GetTick(void)
{
    return system_ticks;
}

/**
 * @brief Manejador de interrupción del temporizador SysTick.
 *        Esta función se ejecuta cada vez que ocurre una interrupción de SysTick.
 */
void SysTick_Handler(void)
{
    system_ticks++;
}

/**
 * @brief Habilita periféricos en el bus AHB2.
 * @param peripherals: Máscara de los periféricos a habilitar.
 */
void RCC_EnableAHB2(uint32_t peripherals)
{
    RCC_AHB2ENR |= peripherals;
}

/**
 * @brief Habilita periféricos en el bus APB2.
 * @param peripherals: Máscara de los periféricos a habilitar.
 */
void RCC_EnableAPB2(uint32_t peripherals)
{
    RCC_APB2ENR |= peripherals;
}

/**
 * @brief Configura un pin GPIO como salida.
 * @param gpio_base: Dirección base del puerto GPIO.
 * @param pin: Número de pin (0-15).
 */
void GPIO_SetPinAsOutput(uint32_t gpio_base, uint32_t pin)
{
    GPIO_MODER(gpio_base) &= ~(0x3U << (pin * 2)); // Limpiar bits
    GPIO_MODER(gpio_base) |= (0x1U << (pin * 2));  // Configurar como salida
}

/**
 * @brief Establece un pin GPIO a nivel alto.
 * @param gpio_base: Dirección base del puerto GPIO.
 * @param pin: Número de pin (0-15).
 */
void GPIO_SetPin(uint32_t gpio_base, uint32_t pin)
{
    GPIO_ODR(gpio_base) |= (1U << pin);
}

/**
 * @brief Establece un pin GPIO a nivel bajo.
 * @param gpio_base: Dirección base del puerto GPIO.
 * @param pin: Número de pin (0-15).
 */
void GPIO_ClearPin(uint32_t gpio_base, uint32_t pin)
{
    GPIO_ODR(gpio_base) &= ~(1U << pin);
}

/**
 * @brief Cambia el estado de un pin GPIO.
 * @param gpio_base: Dirección base del puerto GPIO.
 * @param pin: Número de pin (0-15).
 */
void GPIO_TogglePin(uint32_t gpio_base, uint32_t pin)
{
    GPIO_ODR(gpio_base) ^= (1U << pin);
}
