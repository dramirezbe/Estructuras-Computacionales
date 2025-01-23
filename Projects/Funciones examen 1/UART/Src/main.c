#include <stdint.h>

#include "systick.h"
#include "gpio.h"
#include "uart.h"


int main(void)
{
    configure_systick_and_start();
    configure_gpio();

    init_gpio_pin(GPIOA, 5, 1); // Set PA5 as output
    init_gpio_pin(GPIOC, 13, 0); // Set PC13 as input
    
    UART_Init(USART2);

    UART_send_string(USART2, "Connected...\r\n");

    uint8_t buffer[1];

    UART_receive_it(USART2, buffer, 1);
    while (1) {
        if (rx_ready != 0) {
            UART_send_string(USART2, "Received: ");
            UART_send_string(USART2, (char *)buffer);
            UART_send_string(USART2, "\r\n");
            UART_receive_it(USART2, buffer, 1);
            rx_ready = 0;
        }        
    }

}
