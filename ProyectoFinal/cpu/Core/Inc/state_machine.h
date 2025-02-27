#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "main.h"


uint8_t handle_rb_state(ring_buffer_t *rb);
void handle_keypad_instruction(ring_buffer_t *rb);
uint8_t handle_password(ring_buffer_t *rb);

#endif /* INC_RING_BUFFER_H_ */