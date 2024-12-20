/*
 * led_on_button.s
 *
 *  Created on: Sep 12, 2022
 *      Author: gosor
 *  Led_Control
 */
.syntax unified

.global main

.section .text.main
.type    main,%function
main:

// ACTIVAR RELOJ DE LOS PUERTOS A Y C(bits 0 y 2 en el registro AHB2ENR) (RM0351, page 251)
    ldr r6, =0x40021000  // Carga en el registro 6 (r6) la direccion RCC_BASE (0x40021000)
    ldr r5, [r6,#0x4C]   // Utilizamos otro registro (r5) para sumarle el offset del registro AHB2ENR (0x4C)
    orr r5, 0x00000005   // Activamos el reloj de los puertos A y C colocando unos en los bits 0 y 2utilizando un enmascaramiento con orr
    str r5, [r6,#0x4C]   // Almacenamos el valor del segundo registro en el primer registro el cual contiene la direccion del registro AHB2ENR

// CONFIGURAR EL PIN A5 COMO SALIDA (01) (bits 1:0 in MODER register)
    ldr r6, =0x48000000  // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    ldr r5, [r6,#0x00]   // Utilizamos otro registro(r5) para sumarle el offset del registro MODER(0x00)
    orr r5, #(1<<10)     //  los bits 10 y 11 que corresponden al pin 5 (RM0351, page 303)
    bfc r5, #11, #1      // Colocamos el pin 11 como 0 y el pin 10 como 1 para configurar el pin 5 como salida (RM0351, page 303)
    str r5, [r6,#0x00]     // Cargamos el valor del segundo registro en el primero el cual contiene la direccion GPIOA MODER register

//Encendemos el pin A5 colocanco en ALTOHIGH el pin 5 (RM0351, page 306, 8.5.6)
    ldr r6, =0x48000000  // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    ldr r5, [r6,#0x14]   // Utilizamos otro registro(r5) para sumarle el offset del registro ODR(0x14)
    orr r5, #(1<<5)      // Colocamos en 1 el bit 5 que corresponde al pin 5
    str r5, [r6,#0x14]     // Cargamos el valor del segundo registro en el primero el cual contiene la direccion GPIOA ODR register


// CONFIGURAR EL PIN C13 COMO SALIDA (01) (bits 1:0 in MODER register)
    ldr r6, =0x48000800  // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    ldr r5, [r6,#0x00]   // Utilizamos otro registro(r5) para sumarle el offset del registro MODER(0x00)
    bfc r5, #26, #2      // Colocamos pines 26 y 27 para configurar el pin 5 como salida (RM0351, page 303)
    str r5, [r6,#0x00]     // Cargamos el valor del segundo registro en el primero el cual contiene la direccion GPIOA MODER register


loop:
// Leer PC13(RM0351, page 306, 8.5.6)
//if_pb:
    ldr r6, =0x48000800  // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    ldr r5, [r6,#0x10]   // Utilizamos otro registro(r5) para sumarle el offset del registro ODR(0x14)
    and r5, #(1<<13)      // Colocamos en 1 el bit 5 que corresponde al pin 5
    cbnz r5, if_pb_0
if_pb_1:
     //Rotamos el pin 5 haciendo una or-exclusiva con una máscara en el pin 5
    ldr r6, =0x48000000  // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    movw r5, #(1<<5)      // Colocamos en 1 el bit 5 que corresponde al pin 5
    str r5, [r6,#0x18]     // Cargamos el valor del segundo registro en el primero el cual contiene la direccion GPIOA ODR register
    b end_if
if_pb_0:
     //Rotamos el pin 5 haciendo una or-exclusiva con una máscara en el pin 5
    ldr r6, =0x48000000  // Cargamos en el registro 6 (r6) la direccion GPIOA_MODER(0x48000000)
    movw r5, (1<<5)      // Colocamos en 1 el bit 5 que corresponde al pin 5
    str r5, [r6,#0x28]     // Cargamos el valor del segundo registro en el primero el cual contiene la direccion GPIOA ODR register
end_if:
    b loop
.size    main, .-main