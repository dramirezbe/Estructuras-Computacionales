# Estructuras-Computacionales

## Librería necesaria en ubuntu
sudo apt-get install libncurses6

## Cómo borrar Flash y cargar .elf

Erase = st-flash erase
Read = st-flash write build/debug/programa.bin 0x8000000
Status conection = st-link = st-info --probe

.git
ESTRUCTURAS-COMPUTACIONALES
--Apuntes
--datasheets
--Projects
----project_name1
------.vscode
--------.
------build
--------.
------cmake
--------.
------Inc
--------.
------Src
--------.
------Startup
------CMakeLists.txt
------CMakePresets.json
----project_name2