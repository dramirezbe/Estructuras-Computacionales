# Explicacion examen

- 3 botones y 3 leds exteriores

- 1 boton direccional izquiera
- 2 boton direccional derecha
- 3 boton estacionaria (parpadeo 1 y 2)
- 1 led izquierda
- 2 led derecha
- 3 led heartbeat (parpadeando a cierta frecuencia)

Por UART:
- Ver mensaje de estado (lef, right, stationary)
- poder mandar comandos ("L", "R", "S", "O")  "O" = apaga los dos led

## Funcionamiento:

- 1 Press = blink 3 times
- 2 Press = blink inifinite times
- 1 Press = 3 boton turn off