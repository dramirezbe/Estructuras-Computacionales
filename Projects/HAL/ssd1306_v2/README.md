# Paso a Paso instalar librería externa

1. Descargar driver (https://github.com/afiskon/stm32-ssd1306)
2. Agregar unzip a carpeta External dentro de proyecto
3. Configurar el Driver (driver-name_config.c)
4. Inicializar nuevos periféricos (STM32CUBEMX) 
5. Utilizar el diver en el programa



# Caso ssd1306
1. Descargar driver
2. Agregar unzip a carpeta External dentro de proyecto (carpeta ssd1306)
3. Configurar el Driver (ssd1306_config.c) cambiar nombre de ssd1306_conf_template.c a ssd1306_conf.c
4. Inicializar nuevos periféricos (STM32CUBEMX) (I2c1 -> I2C)