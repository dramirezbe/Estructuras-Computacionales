.syntax unified
.global main

.section .bss
ms_counter:  .space 4           @ Reservar una palabra para el contador de milisegundos en RAM

.section .text
.equ STK_BASE, 0xE000E010       @ Dirección base del bloque de sistema (PM0214, Pag 246)
.equ STK_CTRL, STK_BASE + 0x00  @ Dirección del Registro de Control de SysTick
.equ STK_LOAD, STK_BASE + 0x04  @ Dirección del Registro de Valor de Recarga de SysTick

main:
    ldr r6, =ms_counter     @ Cargar la dirección de ms_counter en r6
    mov r0, #0              @ Establecer R0 en 0
    str r0, [r6]            @ Inicializar ms_counter a 0 (r6 contiene la dirección de ms_counter)

    ldr r7, =STK_CTRL       @ Cargar la dirección del Registro de Control de SysTick en r7
    ldr r1, =STK_LOAD       @ Cargar la dirección del Registro de Valor de Recarga de SysTick en r7
   
    mov r0, #0x4            @ Configurar SysTick para usar el reloj del procesador, deshabilitar temporalmente
    str r0, [r7]            @ Deshabilitar SysTick para configurarlo (r7 contiene STK_CTRL)

    mov r0, #3999           @ Establecer el valor de recarga para 1ms (suponiendo un reloj de sistema de 4 MHz)
    str r0, [r1]            @ Cargar el valor de recarga (r1 contiene STK_LOAD)

    mov r0, #0x5            @ Habilitar SysTick sin interrupciones y con el reloj del procesador
    str r0, [r7]            @ Iniciar SysTick (r7 contiene STK_CTRL)

loop:
    ldr r0, [r7]            @ Leer el Registro de Control de SysTick (r7 contiene la dirección de STK_CTRL)
    tst r0, #0x00010000     @ Probar el bit COUNTFLAG
    beq loop                @ Si COUNTFLAG no está establecido, seguir esperando

    ldr r0, [r6]            @ Leer el valor actual de ms_counter (r6 contiene la dirección de ms_counter)
    add r0, r0, #1          @ Incrementar el contador de milisegundos
    str r0, [r6]            @ Almacenar el valor actualizado en ms_counter
    b loop                  @ Permanecer en este bucle indefinidamente

.end