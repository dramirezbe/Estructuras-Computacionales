#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <stdint.h>

uint8_t button_driver_get_event(void);
void detect_button_press(void);

#endif // __BUTTON_H_