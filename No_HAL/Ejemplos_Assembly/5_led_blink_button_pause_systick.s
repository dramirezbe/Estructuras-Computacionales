.syntax unified
.global main

.section .bss
ms_counter:  .space 4      @ Reservar una palabra para el contador de milisegundos en RAM

.section .text
.equ STK_BASE, 0xE000E010      @ Dirección base del bloque de sistema (PM0214, Pag 246)
.equ STK_CTRL, STK_BASE + 0x00 @ Dirección del Registro de Control de SysTick (PM0214, Pag 246)
.equ STK_LOAD, STK_BASE + 0x04 @ Dirección del Registro de Valor de Recarga de SysTick

.equ RCC_BASE, 0x40021000       @ Dirección base del RCC (RM0351, pag 79)
.equ RCC_AHB2ENR, RCC_BASE + 0x4C @ AHB2ENR para habilitar el GPIOA (RM0351, pag 251)

.equ GPIOA_BASE, 0x48000000         @ Dirección base de GPIOA (RM0351, pag 78)
.equ GPIOA_MODER, GPIOA_BASE + 0x00 @ Modo del puerto GPIOA (RM0351, pag 304)
.equ GPIOA_ODR, GPIOA_BASE + 0x14   @ Registro de salida del puerto GPIOA
.equ LED_PIN, 5                     @ Pin 5 del puerto GPIOA para el LED

.equ GPIOC_BASE, 0x48000800         @ Dirección base de GPIOC (RM0351, pag 78)
.equ GPIOC_MODER, GPIOC_BASE + 0x00 @ Modo del puerto GPIOC (RM0351, pag 304)
.equ GPIOC_IDR, GPIOC_BASE + 0x10   @ Registro de entreda del puerto GPIOC
.equ BUTTON_PIN, 13                     @ Pin 13 del puerto GPIOC para el Button

main:
    bl init_ms_counter              @ Inicializar el contador de milisegundos
    bl configure_systick_and_start  @ Configurar y arrancar SysTick
    bl configure_gpio               @ Habilitar clock y configurar el GPIO para el LED

loop:
    ldr r0, [r6]            @ Leer el Registro de Control de SysTick (r6 contiene la dirección de STK_CTRL)
    tst r0, #0x00010000     @ Probar el bit COUNTFLAG
    beq loop                @ Si COUNTFLAG no está establecido, seguir esperando

    bl increase_ms_counter_and_blink  @ Incrementar el contador de milisegundos
    b loop                  @ Permanecer en este bucle indefinidamente

@ Inicializar el contador de milisegundos
init_ms_counter:
    ldr r5, =ms_counter     @ Cargar la dirección de ms_counter en r5
    mov r0, #0              @ Establecer R0 en 0
    str r0, [r5]            @ Inicializar ms_counter a 0 (r5 contiene la dirección de ms_counter)
    bx lr                   @ Retornar de la función

@ Configurar y arrancar SysTick
configure_systick_and_start:
    ldr r6, =STK_CTRL       @ Cargar la dirección del Registro de Control de SysTick en r6
    ldr r1, =STK_LOAD       @ Cargar la dirección del Registro de Valor de Recarga de SysTick en r1

    mov r0, #0x4            @ Configurar SysTick para usar el reloj del procesador, deshabilitar temporalmente
    str r0, [r6]            @ Deshabilitar SysTick para configurarlo (r6 contiene STK_CTRL)

    mov r0, #3999           @ Establecer el valor de recarga para 1ms (suponiendo un reloj de sistema de 4 MHz)
    str r0, [r1]            @ Cargar el valor de recarga (r1 contiene STK_LOAD)

    mov r0, #0x5            @ Habilitar SysTick sin interrupciones y con el reloj del procesador
    str r0, [r6]            @ Iniciar SysTick (r6 contiene STK_CTRL)
    bx lr                   @ Retornar de la función

@ Configurar GPIO y habilitar el clock para GPIOA
configure_gpio:
    ldr r7, =GPIOA_ODR      @ Cargar la dirección del Registro de Salida GPIOA_ODR en r7
    ldr r0, =RCC_AHB2ENR    @ Cargar la dirección del registro RCC_AHBENR
    ldr r1, [r0]            @ Leer el valor actual del registro RCC_AHBENR
    orr r1, r1, #(1 << 0)   @ Habilitar el clock para GPIOA (bit 0)
    orr r1, r1, #(4 << 0)   @ Habilitar el clock para GPIOC (bit 2)
    str r1, [r0]            @ Almacenar el valor actualizado

    ldr r0, =GPIOA_MODER    @ Cargar la dirección del registro MODER del GPIOA
    ldr r1, [r0]            @ Leer el valor actual del registro MODER
    bic r1, r1, #(0x3 << (LED_PIN * 2))  @ Limpiar los bits correspondientes al pin 5
    orr r1, r1, #(0x1 << (LED_PIN * 2))  @ Configurar el pin 5 como salida (01)
    str r1, [r0]            @ Almacenar el nuevo valor en el registro MODER

    ldr r0, =GPIOC_MODER    @ Cargar la dirección del registro MODER del GPIOC
    ldr r1, [r0]            @ Leer el valor actual del registro MODER
    bic r1, r1, #(0x3 << (BUTTON_PIN * 2))  @ Limpiar los bits correspondientes al pin 13
    orr r1, r1, #(0x0 << (BUTTON_PIN * 2))  @ Configurar el pin 13 como entrada (00)
    str r1, [r0]            @ Almacenar el nuevo valor en el registro MODER


    bx lr                   @ Retornar de la función

@ Incrementar el contador de milisegundos cada 1 ms y parpadear el LED cada 500 ms
increase_ms_counter_and_blink:
    ldr r0, [r5]            @ Leer el valor actual de ms_counter (r5 contiene la dirección de ms_counter)
    add r0, r0, #1          @ Incrementar el contador de milisegundos
    str r0, [r5]            @ Almacenar el valor actualizado en ms_counter

    cmp r0, #500            @ Comparar si han pasado 500 ms desde el último parpadeo
    blt end_blink           @ Si no han pasado 500 ms, saltar al final

    ldr r2, =GPIOC_IDR
    ldr r3, [r2]
    tst r3, #(1 << BUTTON_PIN)
    beq reset_ms_counter

    ldr r1, [r7]            @ Leer el valor actual del registro ODR (r7 contiene la dirección de GPIOA_ODR)
    eor r1, r1, #(1 << LED_PIN)  @ Alternar el bit correspondiente al pin 5 del LED
    str r1, [r7]            @ Almacenar el nuevo valor en el registro ODR (r7 contiene la dirección de GPIOA_ODR)
reset_ms_counter:
    mov r0, #0              @ Reiniciar el contador de milisegundos para el parpadeo
    str r0, [r5]            @ Reiniciar ms_counter a 0 después del parpadeo (r5 contiene la dirección de ms_counter)

end_blink:
    bx lr                   @ Retornar de la función

.end