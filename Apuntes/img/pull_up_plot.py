import matplotlib.pyplot as plt
import numpy as np

# Tiempo en milisegundos
time = np.linspace(0, 100, 500)  # 500 puntos de datos entre 0 ms y 100 ms

# Simular el comportamiento del botón
# Inicial: 1 (alto), durante la pulsación (20ms a 40ms): 0 (bajo)
signal = np.ones_like(time)  # Todo en alto
signal[(time > 20) & (time < 40)] = 0  # En bajo entre 20ms y 40ms

# Graficar
plt.figure(figsize=(8, 4))
plt.plot(time, signal, label="Voltaje del botón (Pull-Up)", color="blue")
plt.axhline(1, color="gray", linestyle="--", linewidth=0.8, alpha=0.7)
plt.axhline(0, color="gray", linestyle="--", linewidth=0.8, alpha=0.7)
plt.title("Comportamiento del botón con Pull-Up")
plt.xlabel("Tiempo (ms)")
plt.ylabel("Voltaje (V)")
plt.ylim(-0.5, 1.5)
plt.yticks([0, 1], ["0V", "Nivel Alto"])
plt.grid(alpha=0.3)
plt.legend()
plt.tight_layout()
plt.show()
