#include <stdint.h>

// Estructura para manejar la tarea de parpadeo
typedef struct {
    uint8_t led_pin;         // LED a controlar
    uint8_t count;           // Contador de toggles
    uint32_t last_tick;      // Marca de tiempo del último toggle
    uint8_t active;          // Indicador de estado (activo o no)
} LedToggleTask;

// Inicializa una tarea de parpadeo
void start_toggle_task(LedToggleTask *task, uint8_t led_pin);

// Maneja una tarea de parpadeo de manera no bloqueante
void handle_toggle_task(LedToggleTask *task, uint32_t current_tick);
