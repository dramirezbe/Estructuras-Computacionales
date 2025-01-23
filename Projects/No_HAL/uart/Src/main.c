#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "uart.h"

int main(void)
{
    configure_systick_and_start();
    configure_gpio();
    UART_Init(USART2); // Inicializa UART en USART2 a 9600 baudios

    uint8_t state = 0; // Estado de la FSM
    uint8_t rx_buffer[32]; // Buffer para almacenar los datos recibidos
    uint8_t rx_index = 0; // Índice para el buffer

    UART_send_string(USART2, "Hello World, from main!\r\n");

    while (1) {
        // Leer un carácter del puerto serial
        if (UART_Receive_Ready(USART2)) { // Verifica si hay datos disponibles
            char received_char = UART_Receive_Char(USART2); // Recibe un carácter

            // Si se recibe el carácter de nueva línea, procesa el mensaje
            if (received_char == '\n' || received_char == '\r') {
                rx_buffer[rx_index] = '\0'; // Finaliza el mensaje recibido
                UART_send_string(USART2, "Mensaje recibido: ");
                UART_send_string(USART2, (char *)rx_buffer);
                UART_send_string(USART2, "\r\n");

                // Verificar si el mensaje es "Mensaje"
                if (strcmp((char *)rx_buffer, "Mensaje") == 0) {
                    UART_send_string(USART2, "¡Comunicacion exitosa!\r\n");
                } else {
                    UART_send_string(USART2, "Mensaje no reconocido\r\n");
                }

                // Reinicia el índice del buffer
                rx_index = 0;
            } else {
                // Almacena el carácter en el buffer si hay espacio
                if (rx_index < sizeof(rx_buffer) - 1) {
                    rx_buffer[rx_index++] = received_char;
                }
            }
        }

        // Cambiar el estado periódicamente como ejemplo
        if (systick_GetTick() >= 10000) {
            state = 0;
            UART_send_string(USART2, "Connected..\r\n");
        }
    }
}
