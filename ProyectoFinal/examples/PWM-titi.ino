#include "Arduino.h"

// Definición de pines
const int Triac = 25;       // Salida al MOC3021 (control del TRIAC)
const int Pin_Cruce = 21;   // Entrada del detector de cruce por cero (4N25)
const int Led_Cruce = 2;    // LED integrado del ESP32

// Variables globales
volatile bool cruce_cero = false;  // Bandera para detectar el cruce por cero
int dim = 0;  // Valor inicial del ángulo de disparo (0-99)
int periodo = 8333;  // Período de la señal de 60 Hz en microsegundos (medio ciclo = 8333 µs)

// Función de interrupción para el cruce por cero
void IRAM_ATTR deteccion_Cruce_cero() {
  cruce_cero = true;  // Activa la bandera de cruce por cero
}

void setup() {
  // Configuración de pines
  pinMode(Triac, OUTPUT);
  pinMode(Pin_Cruce, INPUT);
  pinMode(Led_Cruce, OUTPUT);

  // Configuración de la interrupción del cruce por cero
  attachInterrupt(digitalPinToInterrupt(Pin_Cruce), deteccion_Cruce_cero, RISING);

  // Inicialización del puerto serial
  Serial.begin(115200);
  Serial.println("Control de luminosidad con TRIAC");
}

void loop() {
  // Si se detecta un cruce por cero
  if (cruce_cero) {
    digitalWrite(Led_Cruce, HIGH);  // Enciende el LED (indicador de cruce por cero)
    digitalWrite(Triac, LOW);       // Apaga el TRIAC (inicialmente)

    // Si dim = 0, dispara el TRIAC inmediatamente (deja pasar toda la onda)
    if (dim == 0) {
      digitalWrite(Triac, HIGH);  // Enciende el TRIAC
    }
    // Si dim > 0, calcula el retardo para el ángulo de disparo
    else {
      int retardo = map(dim, 1, 99, 100, periodo - 100);  // Mapea el ángulo (1-99) a un retardo en microsegundos
      delayMicroseconds(retardo);  // Retardo para el desfase controlado
      digitalWrite(Triac, HIGH);   // Enciende el TRIAC
    }

    // Espera un breve momento antes de apagar el TRIAC
    delayMicroseconds(10);         // Pequeño pulso para asegurar el disparo
    digitalWrite(Triac, LOW);      // Apaga el TRIAC

    digitalWrite(Led_Cruce, LOW);  // Apaga el LED
    cruce_cero = false;            // Reinicia la bandera de cruce por cero
  }

  // Ajuste del ángulo de disparo mediante comandos seriales
  if (Serial.available()) {
    char comando = Serial.read();
    if (comando == '+') {
      dim = constrain(dim + 5, 0, 99);  // Aumenta el ángulo
    } else if (comando == '-') {
      dim = constrain(dim - 5, 0, 99);  // Disminuye el ángulo
    }
    Serial.print("Ángulo de disparo: ");
    Serial.println(dim);
  }
}