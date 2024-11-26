# Funcion para inicializar un gpio

```c
void init_gpio_pin(uint32_t *GPIOx_MODER, uint16_t Y_PIN, uint8_t mode) {
    // Clear the current mode bits for the pin
    *GPIOx_MODER &= ~(0x3 << (Y_PIN * 2));  // Clear the two bits corresponding to the pin

    // Set the new mode
    *GPIOx_MODER |= (mode << (Y_PIN * 2));  // Set the new mode
}
```


# Apuntadores

variable result:

- `result` es la variable
- `&result` es la dirección en memoria
- `*result` es el puntero a la dirección de memoria a result.

## Nota:
 
Hacer funciones con máximo 4 argumentos, ya que al haber más de 4, se guardarán en el registro, haciendo el programa más lento.


# Cómo documentar programas:

[página draw.io](draw.io)