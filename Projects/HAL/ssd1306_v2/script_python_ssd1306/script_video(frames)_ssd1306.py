import cv2
import numpy as np
from PIL import Image
import os

def convert_png_to_c_array(image_path, output_file, variable_name, width=128, height=64):
    """
    Convierte una imagen PNG a un array C en formato ROW-MAJOR para ssd1306_DrawBitmap()
    Incluye nombre de variable único para cada frame
    """
    img = Image.open(image_path).convert('1')  # Convertir a monocromo
    img = img.resize((width, height))          # Redimensionar si es necesario

    img_data = np.array(img)

    byte_width = (width + 7) // 8  # Bytes por fila
    byte_array = [0] * (byte_width * height)

    for y in range(height):
        for x in range(width):
            byte_index = y * byte_width + (x // 8)
            bit_position = 7 - (x % 8)  # MSB first

            if img_data[y, x] == 0:  # Pixel negro (ON)
                byte_array[byte_index] |= (1 << bit_position)

    # Generar código C con nombre único
    c_code = f"const uint8_t {variable_name}[{len(byte_array)}] = {{\n"
    for i, byte in enumerate(byte_array):
        c_code += f"0x{byte:02X},"
        if (i + 1) % 16 == 0:  # Nueva línea cada 16 bytes
            c_code += "\n"
    c_code += "\n};\n"

    with open(output_file, 'w') as f:
        f.write(c_code)

def process_video(video_path, output_dir, fps=15, width=128, height=64):
    """
    Procesa un video y genera arrays C para cada frame
    :param video_path: Ruta del video de entrada
    :param output_dir: Directorio de salida para los archivos .h
    :param fps: Fotogramas por segundo a extraer
    :param width: Ancho de la pantalla OLED
    :param height: Alto de la pantalla OLED
    """
    # Crear directorio de salida si no existe
    os.makedirs(output_dir, exist_ok=True)
    
    # Abrir video
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        raise ValueError("Error al abrir el video")

    # Calcular intervalo entre frames
    video_fps = cap.get(cv2.CAP_PROP_FPS)
    frame_interval = max(1, int(round(video_fps / fps)))
    print(f"Extrayendo 1 frame cada {frame_interval} frames del video")

    frame_count = 0
    saved_count = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Extraer frames según el FPS objetivo
        if frame_count % frame_interval == 0:
            # Convertir a RGB y crear imagen PIL
            pil_img = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
            
            # Guardar temporalmente como PNG
            temp_path = os.path.join(output_dir, f"temp_{saved_count:04d}.png")
            pil_img.save(temp_path)
            
            # Generar nombre único para el array C
            var_name = f"image_data_{saved_count}"
            output_path = os.path.join(output_dir, f"frame_{saved_count:04d}.h")
            
            # Convertir a array C
            convert_png_to_c_array(temp_path, output_path, var_name, width, height)
            
            # Eliminar archivo temporal
            os.remove(temp_path)
            
            saved_count += 1
            print(f"Frame {saved_count} procesado")

        frame_count += 1

    cap.release()
    print(f"\nProceso completado! {saved_count} frames generados")

if __name__ == "__main__":
    # Configuración
    VIDEO_PATH = "y2mate.com - Neon Genesis Evangelion OP Broadcast ver_360p (online-video-cutter.com).mp4"         # Cambiar por tu video
    OUTPUT_DIR = "output_images"          # Directorio de salida
    TARGET_FPS = 2                     # Frames por segundo a extraer
    OLED_WIDTH = 128                    # Ancho pantalla OLED
    OLED_HEIGHT = 64                    # Alto pantalla OLED

    # Ejecutar procesamiento
    process_video(
        video_path=VIDEO_PATH,
        output_dir=OUTPUT_DIR,
        fps=TARGET_FPS,
        width=OLED_WIDTH,
        height=OLED_HEIGHT
    )