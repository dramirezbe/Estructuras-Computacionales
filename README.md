# Estructuras-Computacionales

## Librería necesaria en ubuntu (Solo v5, la v6 no sirve ubuntu 24.0 no tiene v5)

```bash
sudo apt-get install libncurses5 
```

## Cómo borrar Flash y cargar .elf

```bash
st-flash erase
st-flash write build/debug/programa.bin 0x8000000
st-link = st-info --probe
```

## Cómo inicializar SSH de git

```bash
eval "$(ssh-agent -s)"

ssh-add ~/.ssh/id_ed25519

ssh -T git@github.com
```

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
```

## Cómo conectar puerto serial

Evaluar puertos

```bash
sudo dmesg | grep tty
```

Verificar estado del puerto

```bash
ls -l /dev/puerto
```

Ejecutar aplicación a utilizar

```bash
sudo putty
```

Información GPIO

pag301 rm0351


## Contacto Monitor

Juan David Martinez

3118210922

juamartinezs@unal.edu.co
