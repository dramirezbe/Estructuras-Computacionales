.syntax unified
.global main

.section .bss
ms_counter:  .space 4      @ Reservar una palabra (4 bytes) para el contador de milisegundos en la sección .bss

.section .text
.equ STK_BASE, 0xE000E010      @ Dirección base del bloque SysTick
.equ STK_CTRL, STK_BASE + 0x00 @ Dirección del Registro de Control de SysTick
.equ STK_LOAD, STK_BASE + 0x04 @ Dirección del Registro de Recarga de SysTick

.equ RCC_BASE, 0x40021000       @ Dirección base del módulo RCC (controlador del reloj)
.equ RCC_AHB2ENR, RCC_BASE + 0x4C @ Dirección del registro AHB2ENR para habilitar relojes de periféricos

.equ GPIOA_BASE, 0x48000000         @ Dirección base de GPIOA
.equ GPIOA_MODER, GPIOA_BASE + 0x00 @ Dirección del registro MODER de GPIOA (configuración de modo)
.equ GPIOA_ODR, GPIOA_BASE + 0x14   @ Dirección del registro ODR de GPIOA (datos de salida)
.equ LED_PIN, 5                     @ Definir el pin 5 de GPIOA como el pin del LED

.equ GPIOC_BASE, 0x48000800         @ Dirección base de GPIOC
.equ GPIOC_MODER, GPIOC_BASE + 0x00 @ Dirección del registro MODER de GPIOC
.equ GPIOC_IDR, GPIOC_BASE + 0x10   @ Dirección del registro IDR de GPIOC (datos de entrada)
.equ BUTTON_PIN, 13                 @ Definir el pin 13 de GPIOC como el pin del botón azul

main:
    bl init_ms_counter              @ Llamar a la subrutina para inicializar el contador de milisegundos
    bl configure_systick_and_start  @ Configurar y arrancar SysTick
    bl configure_gpio               @ Configurar GPIO para el LED y el botón

loop:
    ldr r0, [r6]                    @ Cargar el contenido del registro SysTick Control en r0
    tst r0, #0x00010000             @ Probar si el bit COUNTFLAG está establecido (indica que SysTick ha contado)
    beq loop                        @ Si COUNTFLAG no está establecido, continuar en el bucle esperando

    ldr r0, =GPIOC_IDR              @ Cargar la dirección del registro IDR de GPIOC (datos de entrada)
    ldr r1, [r0]                    @ Leer el contenido del registro IDR en r1
    tst r1, #(1 << BUTTON_PIN)      @ Probar el bit correspondiente al pin del botón (PC13)
    bne button_not_pressed          @ Si el botón no está presionado (bit alto), saltar a la etiqueta `button_not_pressed`

    b loop                          @ Si el botón está presionado, regresar al inicio del bucle

button_not_pressed:
    bl increase_ms_counter_and_blink  @ Llamar a la subrutina para incrementar el contador y parpadear el LED
    b loop                          @ Regresar al inicio del bucle principal

@ Subrutina para inicializar el contador de milisegundos
init_ms_counter:
    ldr r5, =ms_counter             @ Cargar la dirección de `ms_counter` en r5
    mov r0, #0                      @ Establecer el valor 0 en r0
    str r0, [r5]                    @ Escribir 0 en la ubicación de memoria apuntada por r5
    bx lr                           @ Retornar de la subrutina

@ Subrutina para configurar y arrancar SysTick
configure_systick_and_start:
    ldr r6, =STK_CTRL               @ Cargar la dirección del registro SysTick Control en r6
    ldr r1, =STK_LOAD               @ Cargar la dirección del registro SysTick Load en r1

    mov r0, #0x4                    @ Configurar SysTick para usar el reloj del procesador y deshabilitarlo temporalmente
    str r0, [r6]                    @ Escribir el valor en el registro de control de SysTick

    mov r0, #3999                   @ Establecer el valor de recarga para 1 ms (asumiendo un reloj de 4 MHz)
    str r0, [r1]                    @ Escribir el valor de recarga en el registro SysTick Load

    mov r0, #0x5                    @ Habilitar SysTick con el reloj del procesador y sin interrupciones
    str r0, [r6]                    @ Escribir en el registro de control de SysTick para iniciarlo
    bx lr                           @ Retornar de la subrutina

@ Subrutina para configurar GPIO para el LED y el botón
configure_gpio:
    ldr r7, =GPIOA_ODR              @ Cargar la dirección del registro ODR de GPIOA en r7
    ldr r0, =RCC_AHB2ENR            @ Cargar la dirección del registro AHB2ENR en r0
    ldr r1, [r0]                    @ Leer el valor actual del registro AHB2ENR
    orr r1, r1, #(1 << 0)           @ Habilitar el reloj para GPIOA (bit 0)
    orr r1, r1, #(1 << 2)           @ Habilitar el reloj para GPIOC (bit 2)
    str r1, [r0]                    @ Escribir el valor actualizado en el registro AHB2ENR

    ldr r0, =GPIOA_MODER            @ Cargar la dirección del registro MODER de GPIOA
    ldr r1, [r0]                    @ Leer el valor actual del registro MODER
    bic r1, r1, #(0x3 << (LED_PIN * 2))  @ Limpiar los bits correspondientes al pin del LED
    orr r1, r1, #(0x1 << (LED_PIN * 2))  @ Configurar el pin del LED como salida (modo 01)
    str r1, [r0]                    @ Escribir el nuevo valor en el registro MODER

    ldr r0, =GPIOC_MODER            @ Cargar la dirección del registro MODER de GPIOC
    ldr r1, [r0]                    @ Leer el valor actual del registro MODER
    bic r1, r1, #(0x3 << (BUTTON_PIN * 2))  @ Configurar el pin del botón (PC13) como entrada (modo 00)
    str r1, [r0]                    @ Escribir el valor actualizado en el registro MODER
    bx lr                           @ Retornar de la subrutina

@ Subrutina para incrementar el contador y controlar el parpadeo del LED
increase_ms_counter_and_blink:
    ldr r0, [r5]                    @ Cargar el valor actual del contador de milisegundos en r0
    add r0, r0, #1                  @ Incrementar el contador en 1
    str r0, [r5]                    @ Escribir el valor actualizado en la ubicación de memoria

    cmp r0, #500                    @ Comparar si han pasado 500 ms
    blt end_blink                   @ Si no han pasado 500 ms, saltar al final

    ldr r1, [r7]                    @ Leer el valor actual del registro ODR de GPIOA
    eor r1, r1, #(1 << LED_PIN)     @ Alternar el bit correspondiente al LED
    str r1, [r7]                    @ Escribir el nuevo valor en el registro ODR

    mov r0, #0                      @ Reiniciar el contador de milisegundos
    str r0, [r5]                    @ Escribir 0 en la ubicación de memoria del contador

end_blink:
    bx lr                           @ Retornar de la subrutina

.end
