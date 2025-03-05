# Laboratorio 2 - Drivers Embebidos y Optimización

**Curso:** 4100901 - Computation Structures  
**Autores:** Samuel Cifuentes

---

## 0. Clonar el Repositorio

Para iniciar, descarga o clona el repositorio del laboratorio:

```bash
git clone https://github.com/saacifuentesmu/4100901-Lab-2.git
```

---

## 1. Importar el Proyecto en VS Code

Utiliza la extensión **STM32 for VS Code** para importar el proyecto:

1. Abre VS Code.
2. Asegúrate de que la extensión STM32 esté instalada y configurada.
3. Selecciona la opción de importar proyecto CMake y abre en la misma ventana.
4. Verifica que se hayan cargado correctamente todos los archivos.

---

## 2. Pasos para el Desarrollo de la Práctica de Laboratorio

La práctica de este laboratorio se centra en tres aspectos principales:

### A. Medición del Tiempo de Transmisión a la Pantalla OLED

- **Objetivo:** Medir el tiempo de transmisión de un frame hacia la pantalla OLED SSD1306 mediante el uso de un analizador lógico.
- **Pasos:**
  1. **Configuración del Hardware:**
     - Conecta los pines **PB8** y **PB9** de la Nucleo board para la medición de las señales I2C.
     - Conecta la pantalla OLED SSD1306.
  2. **Configuración del Analizador Lógico:**
     - Configura el analizador lógico para capturar y medir las señales I2C.
  3. **Ejecución y Medición:**
     - Ejecuta el código y utiliza el analizador lógico para detectar la transmisión del frame.
     - Registra el tiempo medido.

### B. Medición del Tiempo de Procesamiento de un Evento Externo

- **Objetivo:** Medir el tiempo entre la detección de un evento externo y la actualización de la pantalla OLED.
- **Pasos:**
  1. **Configuración del Hardware:**
     - Utiliza el pin **PB1** de la Nucleo board para detectar un cambio de nivel (evento externo).
  2. **Implementación en Código:**
     - En el código se utiliza una función de callback para detectar la interrupción generada por el botón (como se observa en `HAL_GPIO_EXTI_Callback` en `main.c`).
     - Cada vez que se detecta el evento, se incrementa un contador y se actualiza la pantalla.
  3. **Ejecución y Medición:**
     - Ejecuta el programa y utiliza el analizador lógico para medir el tiempo de respuesta desde el cambio de nivel hasta la actualización visual en la pantalla.
     - Repite los pasos usando las optimizaciones -O0, -O3, -Og, -Os, las cuales se pueden configurar en `cmake/gcc-arm-none-eabi.cmake` en la linea 32.

### C. Medición del Consumo de Corriente en Diferentes Modos

- **Objetivo:** Comparar el consumo de corriente en dos modos de operación del microcontrolador:
  1. **Run Mode**
  2. **Sleep Mode**
- **Pasos:**
  1. **Implementación:**
     - Se debe modificar o utilizar las funciones del firmware para entrar en diferentes modos de operación. Por ejemplo, la función `low_power_sleep_mode()` en `main.c` sirve para poner el dispositivo en modo de bajo consumo.
     - Para habilitar la funcion de low power sleep mode puede usar el macro de la linea 152 en `main.c`
  2. **Medición:**
     - Utiliza un multímetro para medir la corriente consumida en cada uno de los modos.
  3. **Comparación:**
     - Registra y compara los resultados obtenidos para analizar el impacto de cada modo en el consumo.

---

## 3. Reporte de Resultados y Conclusiones

Al finalizar las mediciones y pruebas, el reporte debe incluir:

- **Descripción de la Metodología:** Explicar cómo se realizaron las mediciones y qué herramientas se utilizaron (analizador lógico, multímetro, etc.).
- **Resultados:** Presentar tablas, gráficos y capturas de pantalla de las mediciones realizadas.
- **Análisis y Conclusiones:** Discutir los tiempos de transmisión, tiempos de respuesta al evento externo y las diferencias en el consumo de corriente en cada modo.

**Reglas para el Reporte:**

- El reporte debe ser breve, entendible y contener imágenes junto con explicaciones textuales.
- Se permite la realización del reporte en grupos de máximo dos personas.
- La presentación del reporte se realizará únicamente a quienes hayan demostrado un avance significativo en la sesión de laboratorio (tomará lista el 25 de febrero de 2025).
- Puedes utilizar el formato de archivo que consideres más conveniente (PDF, DOCX, etc.).

---

¡Buena suerte en el laboratorio!
