// Led_Blink_SysTick_s
// pulseview sigrok analizador lógico
//https://zadig.akeo.ie/ para instalar el driver para windows

.syntax unified

.global main
.global SysTick_Handler

.data
contador:    .word         400000
uwTick:     .word         0


.equ SysTick_BASE,0xE000E010
.equ SysTick_CTRL,0x0
.equ SysTick_LOAD,0x4
.equ SysTick_VAL,0x8
.equ SysTick_CALIB,0xC

.equ SCB_BASE,0xE000ED00
.equ SCB_SHP,0x20


// Constants defined in file stm32L476xx_constants.S
//      RCC   base address is 0x40021000
//   AHB2ENR register offset is 0x4C
//      RCC   base address is 0x40021000
//   APB2ENR register offset is 0x60
.equ    RCC_BASE,          0x40021000
.equ    RCC_AHB2ENR,       0x4C // RCC AHB2 peripheral clock reg (RM0351, page 251) Para el pueto A
.equ    RCC_APB2ENR,       0x60 // RCC APB2 peripheral clock reg (RM0351, page 258) Para el SysTick



//      GPIOA base address is 0x48000000
//   MODER register offset is 0x00
//   ODR   register offset is 0x14
.equ    GPIOA_BASE,     0x48000000 // GPIO BASE ADDRESS (RM0351, page 78)
// .equ    GPIOB_BASE,     0x48000400
.equ    GPIOC_BASE,     0x48000800
// .equ    GPIOD_BASE,     0x48000C00
// .equ    GPIOE_BASE,     0x48001000
// .equ    GPIOF_BASE,     0x48001400
// .equ    GPIOG_BASE,     0x48001800
// .equ    GPIOH_BASE,     0x48001C00

.equ    GPIO_MODER,       0x00 // GPIO port mode register (RM0351, page 303)
.equ     GPIO_IDR,         0x10
.equ    GPIO_ODR,         0x14 // GPIO outpu data register (RM0351, page 306)

.section .text.main
.type    main,%function
main:
////////////////////////////////////////////////////////////////

// Enable SYSCFG Controller Clock (bit 0 in APB2ENR register)
    ldr r6, =RCC_BASE       // Load peripheral clock reg address to r6
    ldr r5, [r6,#RCC_APB2ENR]                // Read its content to r5
    orr r5, 0x1                 // Set   bit 0 to enable SYSCFG clock
    str r5, [r6,#RCC_APB2ENR]                // Store result in peripheral clock register

// Set SysTick_CTRL to disabLe SysTick IRQ and SysTick timer
    ldr r0, =SysTick_BASE

    // DisabLe SysTick IRQ and SysTick counter, select external clock
    mov r1, #0
    str r1, [r0, #SysTick_CTRL]

    // Specify the number of cLock cycles between two interrupts
    ldr r2, =3999 // Change it based on interrupt interval
    str r2, [r0, #SysTick_LOAD] // Save to SysTick reload register

    // Clear SysTick current value register (SysTick_VAL)
    mov r1, #0
    str r1, [r0, #SysTick_VAL] // Write e to SysTick value register

    // Set interrupt priority for SysTick
    ldr r2, =SCB_BASE
    add r2, r2, #SCB_SHP
    ldr r1, =0xf0000000    // Set priority as 1, see Figure 11-7
    str r1, [r2]    // SCB->SHP[ll), see Figure 11 -8

    // Set SysTick_CTRL to enable SysTick timer and SysTick interrupt
    LDR r1, [r0, #SysTick_CTRL]
    MOV r2,#0X00000007
    ORR r1,r1,r2  // Enable SysTick counter & interrupt
    STR r1, [r0, #SysTick_CTRL]

    // Habilitar las interrupciones Globales
    CPSIE i


/////////////////////////////////////////////////////////////////
// Enable GPIOA Peripheral Clock (bit 0 in AHB2ENR register) (RM0351, page 251)
    ldr r6, =RCC_BASE       // Load peripheral clock reg address to r6
    ldr r5, [r6,#RCC_AHB2ENR]   // Read its content to r5
    orr r5, 0x00000001    // AHB2 peripheral clock enable register (RCC_AHB2ENR) (RM0351, PAGE 251
    str r5, [r6,#RCC_AHB2ENR]

// Make GPIOA Pin5 as output pin (bits 1:0 in MODER register)
    ldr r6, =GPIOA_BASE       // Load GPIOA base register address to r6
    ldr r5, [r6,#GPIO_MODER]    // Read the content the MODER register to r5
    orr r5, #(1<<10)              // Clear bits 11, 10 for P5 (RM0351, page 303)
    bfc r5, #11, #1              // Write 01 to bits 11, 10 for P5 (RM0351, page 303)
    str r5, [r6,#GPIO_MODER]    // Store result in GPIOA MODER register


//Set pin A5
    ldr r6, =GPIOA_BASE       // Load GPIOA MODER register address to r6
    ldr r5, [r6,#GPIO_ODR]    // Read ODR content to r5
    orr r5, #(1<<5)              // Set P5 (RM0351, page 306)
    str r5, [r6,#GPIO_ODR]    // Store result in GPIOA MODER register



// Enable GPIOC Peripheral Clock (bit 0 in AHB2ENR register) (RM0351, page 251)
    ldr r6, =RCC_BASE       // Load peripheral clock reg address to r6
    ldr r5, [r6,#RCC_AHB2ENR]   // Read its content to r5
    orr r5, 0x00000004    // AHB2 peripheral clock enable register (RCC_AHB2ENR) (RM0351, PAGE 251
    str r5, [r6,#RCC_AHB2ENR]

// Make GPIOC Pin13 as output pin (bits 27:26 in MODER register)
    ldr r6, =GPIOC_BASE       // Load GPIOA MODER register address to r6
    ldr r5, [r6,#GPIO_MODER]              // Read its content to r5
    and r5, #0xF3FFFFFF          // Write 00 to bits 27, 26 for P13
    str r5, [r6]

loop:



//  Set pin A5
    ldr r6, =GPIOA_BASE       // Load GPIOA MODER register address to r6
    ldr r5, [r6,#GPIO_ODR]    // Read ODR content to r5
    orr r5, #(1<<5)              // Set P5 (RM0351, page 306)
    str r5, [r6,#GPIO_ODR]    // Store result in GPIOA MODER register

// delay1:
    ldr r0,=contador
    ldr r1,[r0]
delay1:    sub r1,#1

    cbz r1,enddelay1
    b delay1
enddelay1:

// Clear pin A5
    ldr r6, =GPIOA_BASE
    ldr r5, [r6,#GPIO_ODR]    // Read ODR content to r5
    bfc r5, #5, #1              // Clear PA5
    str r5, [r6,#GPIO_ODR]    // Store result in GPIOA MODER register

/// delay2:
    ldr r0,=contador
    ldr r1,[r0]
delay2:    sub r1,#1

    cbz r1,enddelay2
    b delay2
enddelay2:

    b loop
.size    main, .-main


.section    .text.SysTick_Handler
.type    SysTick_Handler,%function
SysTick_Handler:
    push {lr}
    ldr r0,=uwTick
    ldr r1,[r0]
    add r1,#1
    str r1,[r0]
    pop {pc}
    //bx lr
.size SysTick_Handler, .-SysTick_Handler