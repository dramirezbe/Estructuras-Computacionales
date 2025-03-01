#include "state_machine.h"

#define A_STATE 0
#define B_STATE 1
#define C_STATE 2
#define ERR     3

/**
 * @brief Choose keypad mode (A,B,C), based on rb content, if the function found a valid char,
 *  it will return the state of the keypad and erase the char, mantaning other content
 * @param rb: pointer to ring buffer
 * @retval uint8_t: return the state of the keypad
 */
uint8_t handle_keypad_rb_state(ring_buffer_t *rb) {
    uint8_t key;
    uint8_t temp_buffer[BUFFER_CAPACITY];  // Buffer temporal para almacenar los datos a conservar
    uint8_t temp_count = 0;
    uint8_t found_state = ERR;             // Estado encontrado (ERR por defecto)

    // Si el buffer está vacío, retorna ERR
    if (ring_buffer_is_empty(rb)) {
        return ERR;
    }

    // Recorrer el ring buffer extrayendo cada clave
    while (ring_buffer_read(rb, &key)) {
        // Si aún no se encontró una tecla válida y es A, B o C
        if (found_state == ERR && (key == 'A' || key == 'B' || key == 'C')) {
            if (key == 'A') {
                found_state = A_STATE;
            } else if (key == 'B') {
                found_state = B_STATE;
            } else if (key == 'C') {
                found_state = C_STATE;
            }
            // No se agrega al buffer temporal, por lo que se elimina de la cola
        }
        else {
            // Conservar el resto de caracteres
            temp_buffer[temp_count++] = key;
        }
    }

    // Reinserción de los caracteres que no se procesaron en el ring buffer
    for (uint8_t i = 0; i < temp_count; i++) {
        ring_buffer_write(rb, temp_buffer[i]);
    }

    return found_state;
}

/**
 * @brief Handle the keypad instruction based on the state of the keypad
 * @param None
 */

void handle_keypad_instruction(ring_buffer_t *rb) {
    switch(handle_keypad_rb_state(rb)) 
    {
        case 0:
            if (handle_password(rb)) {
                //unlock_door();
            }
            else {
                //lock_door();
            }

            break;
        case 1:
            //handle_luminosity(rb);
            break;
        case 2:
            //lock_door();
            break;
        case 3:
            break;
        default:
            break;

    }

}

/**
 * @brief Verificar si la contraseña ingresada es correcta
 * @param rb: Puntero al ring buffer
 * @retval 1 si la contraseña es correcta, 0 si no
 */
uint8_t handle_password(ring_buffer_t *rb) {
    // Verificar si el ring buffer tiene exactamente 4 caracteres
    if (ring_buffer_size(rb) != 4 || ring_buffer_is_empty(rb)) {
        return 0;
    }
    
    char temp[5];  // 4 caracteres + '\0'
    uint8_t key;
    
    // Extraer 4 caracteres del ring buffer
    for (uint8_t i = 0; i < 4; i++) {
        ring_buffer_read(rb, &key);
        temp[i] = key;
    }
    temp[4] = '\0'; // Terminar la cadena
    
    // Comparar la cadena obtenida con la contraseña definida
    if (strcmp(temp, PASSWORD) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void unlock_door() {
    door_status = 1;
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}