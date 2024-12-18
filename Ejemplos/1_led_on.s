/*
 * led_on.s
 *
 *  Created on: Aug 24, 2022
 *      Author: Samuel
 */
.syntax unified

.global main


.section .text.main
.type    main,%function
main:

// ACTIVAR RELOJ DEL PUERTO A (bit 0 in AHB2ENR register) (RM0351, page 251)
    ldr r6, =0x40021000  // Carga en el registro 6 (r6) la direccion RCC_BASE (0x40021000)
    ldr r5, [r6,#0x4C]  // Utilizamos otro registro (r5) para sumarle el offset del registro AHB2ENR (0x4C)
    orr r5, 0x00000001 // Activamos el reloj del puerto A colocando un 1 en el bit 0 utilizando un enmascaramiento con orr
    str r5, [r6,#0x4C] // Almacenamos el valor del segundo registro en el primer registro el cual contiene la direccion del registro AHB2ENR

// CONFIGURAR EL PIN 5 COMO SALIDA (01) (bits 1:0 in MODER register)
    ldr r6, =0x48000000    // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    ldr r5, [r6,#0x00]    // Utilizamos otro registro(r5) para sumarle el offset del registro MODER(0x00)
    orr r5, #(1<<10)     //  los bits 10 y 11 que corresponden al pin 5 (RM0351, page 303)
    bfc r5, #11, #1     // Colocamos el pin 11 como 0 y el pin 10 como 1 para configurar el pin 5 como salida (RM0351, page 303)
    str r5, [r6,#0x00]    // Cargamos el valor del segundo registro en el primero el cual contiene la direccion GPIOA MODER register


//Encendemos el pin 5 colocanco en HIGH el pin 5 (RM0351, page 306, 8.5.6)
    ldr r6, =0x48000000       // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    ldr r5, [r6,#0x14]    // Utilizamos otro registro(r5) para sumarle el offset del registro ODR(0x14)
    orr r5, #(1<<5)              // Colocamos en 1 el bit 5 que corresponde al pin 5
    str r5, [r6,#0x14]    // Cargamos el valor del segundo registro en el primero el cual contiene la direccion GPIOA ODR register



loop:
     nop
     nop
     nop
    b loop
.size    main, .-main