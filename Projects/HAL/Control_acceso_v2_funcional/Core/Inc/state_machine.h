#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "main.h"

typedef enum {
    LOCKED,
    TEMP_UNLOCK,
    PERM_UNLOCK
} state_t;

typedef enum {
    CMD_NONE = 0,
    CMD_OPEN = 'O',
    CMD_CLOSE = 'C'
} command_t;

void handle_event(uint8_t event);
void run_state_machine(void);
uint8_t button_driver_get_event(void);

#endif