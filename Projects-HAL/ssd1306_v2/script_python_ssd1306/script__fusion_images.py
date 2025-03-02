import os
import re

def merge_h_files(input_dir, output_file, array_name="frames"):
    """
    Fusiona todos los archivos .h de un directorio en un único archivo
    con estructura de animación para SSD1306.
    
    :param input_dir: Directorio con los archivos .h
    :param output_file: Archivo de salida combinado
    :param array_name: Nombre del array principal de animación
    """
    # Obtener lista de archivos .h ordenados numéricamente
    files = sorted(
        [f for f in os.listdir(input_dir) if f.endswith(".h")],
        key=lambda x: int(re.search(r'\d+', x).group())
    )

    # Encabezado del archivo combinado
    header = f"""\
#ifndef {array_name.upper()}_H
#define {array_name.upper()}_H

#include <stdint.h>

"""

    # Cuerpo principal
    main_content = []
    frame_declarations = []
    
    for i, filename in enumerate(files):
        with open(os.path.join(input_dir, filename), 'r') as f:
            content = f.read()
            
            # Extraer el nombre del array individual
            var_name_match = re.search(r'const uint8_t (\w+)\[', content)
            if var_name_match:
                var_name = var_name_match.group(1)
                frame_declarations.append(var_name)
                
                # Modificar declaración a 'static const' para evitar colisiones
                modified_content = content.replace(
                    'const uint8_t',
                    'static const uint8_t',
                    1
                )
                main_content.append(modified_content)

    # Generar array principal de frames
    main_array = f"""
// Total frames: {len(frame_declarations)}
const uint8_t* {array_name}[] = {{
    {',\n    '.join(frame_declarations)}
}};

const uint32_t {array_name}_count = {len(frame_declarations)};

#endif // {array_name.upper()}_H
"""

    # Escribir archivo final
    with open(output_file, 'w') as f:
        f.write(header)
        f.write('\n'.join(main_content))
        f.write(main_array)

    print(f"Fusionados {len(files)} archivos en {output_file}")
    print(f"Array principal: {array_name} con {len(frame_declarations)} frames")

if __name__ == "__main__":
    # Configuración
    INPUT_DIR = "output_images"       # Directorio con los archivos .h
    OUTPUT_FILE = "animation.h"     # Archivo de salida combinado
    ARRAY_NAME = "evangelion_frames"  # Nombre del array de animación

    # Ejecutar fusión
    merge_h_files(INPUT_DIR, OUTPUT_FILE, ARRAY_NAME)