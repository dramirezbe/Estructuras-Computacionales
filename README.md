# Estructuras-Computacionales

## Librería necesaria en ubuntu
sudo apt-get install libncurses6

## Cómo borrar Flash y cargar .elf

Erase = st-flash erase

Read = st-flash write build/debug/programa.bin 0x8000000

Status conection st-link = st-info --probe

## Cómo inicializar SSH de git

eval "$(ssh-agent -s)"

ssh-add ~/.ssh/id_ed25519

ssh -T git@github.com


```markdown
- .git  
- Apuntes
- datasheets
- Projects
- project_name_1
    - .vscode
    - build
    - cmake
    - Inc
        - functions.h
    - Src
        - main.c
    - Startup
    - CMakeLists.txt
    - CMakePresets.json
    - stm32l476rgtx_FLASH.ld
- project_name_2
    - .vscode
    - build
    - cmake
    - Inc
        - functions.h
    - Src
        - main.c
    - Startup
    - CMakeLists.txt
    - CMakePresets.json
    - stm32l476rgtx_FLASH.ld
