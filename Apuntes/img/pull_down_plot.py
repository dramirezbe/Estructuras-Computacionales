import matplotlib.pyplot as plt
import numpy as np

# Tiempo en milisegundos
time = np.linspace(0, 100, 500)  # 500 puntos de datos entre 0 ms y 100 ms

# Simular el comportamiento del botón
# Inicial: 0 (bajo), durante la pulsación (20ms a 40ms): 1 (alto)
signal = np.zeros_like(time)  # Todo en bajo
signal[(time > 20) & (time < 40)] = 1  # En alto entre 20ms y 40ms

# Graficar
plt.figure(figsize=(8, 4))
plt.plot(time, signal, label="Voltaje del botón (Pull-Down)", color="green")
plt.axhline(1, color="gray", linestyle="--", linewidth=0.8, alpha=0.7)
plt.axhline(0, color="gray", linestyle="--", linewidth=0.8, alpha=0.7)
plt.title("Comportamiento del botón con Pull-Down")
plt.xlabel("Tiempo (ms)")
plt.ylabel("Voltaje (V)")
plt.ylim(-0.5, 1.5)
plt.yticks([0, 1], ["Nivel Bajo", "3.3V"])
plt.grid(alpha=0.3)
plt.legend()
plt.tight_layout()
plt.show()
