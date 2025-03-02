#include "systick.h"

#define SysTick ((SysTick_t *)0xE000E010) // Base address of SysTick


volatile uint32_t ms_counter_a = 0; // Counter for milliseconds (A)
volatile uint32_t ms_counter_b = 0; // Counter for milliseconds (B)


void configure_systick_and_start(void)
{
    SysTick->CTRL = 0x4;     // Disable SysTick for configuration, use processor clock
    SysTick->LOAD = 3999;    // Reload value for 1 ms (assuming 4 MHz clock)
    SysTick->CTRL = 0x7;     // Enable SysTick, processor clock, no interrupt
}

uint32_t systick_GetTick(Counter_t counter)
{
    switch (counter) {
        case COUNTER_A:
            return ms_counter_a;
        case COUNTER_B:
            return ms_counter_b;
        default:
            return 0; // Default case if an invalid counter is used
    }
}

void systick_reset(Counter_t counter)
{
    switch (counter) {
        case COUNTER_A:
            ms_counter_a = 0;
            break;
        case COUNTER_B:
            ms_counter_b = 0;
            break;
        default:
            // Invalid counter; do nothing
            break;
    }
}

void SysTick_Handler(void)
{
    ms_counter_a++; // Increment counter A
    ms_counter_b++; // Increment counter B
}
