<<<<<<< HEAD


#include <stdint.h>

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;

} SysTick_t;

typedef enum {
    COUNTER_A,
    COUNTER_B
} Counter_t;

void configure_systick_and_start(void);
uint32_t systick_GetTick(Counter_t COUNTER_A);
void systick_reset(Counter_t COUNTER_B);
=======


#include <stdint.h>

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;

} SysTick_t;

typedef enum {
    COUNTER_A,
    COUNTER_B
} Counter_t;

void configure_systick_and_start(void);
uint32_t systick_GetTick(Counter_t COUNTER_A);
void systick_reset(Counter_t COUNTER_B);
>>>>>>> af4b251ca32924dfacd5b62a67d5a1a332606774
