#include <ESP32Servo.h>

// Pines del sensor ultrasónico
const int EchoPin = 33;
const int TriggerPin = 25;

// Pines de los motores
const int motores[4] = {12, 14, 27, 26}; // DelanteroAdelante, DelanteroAtras, TraseroAdelante, TraseroAtras

// Pines y configuración del servo
#define PINSERVO 32
Servo servoMotor;
const int anguloMin = 0;   // Ángulo mínimo
const int anguloMax = 180; // Ángulo máximo

void setup() {
  Serial.begin(115200);

  // Configurar pines de motores como salidas
  for (int i = 0; i < 4; i++) {
    pinMode(motores[i], OUTPUT);
  }

  // Configurar pines del sensor ultrasónico
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Iniciar el servo
  servoMotor.attach(PINSERVO);
}

void loop() {
  float distancia = ping();
  Serial.print("Distancia: ");
  Serial.println(distancia);

  if (distancia > 45.0) {
    moverMotores(HIGH, LOW, HIGH, LOW, 500); // Avanzar
  } else {
    detenerse(500);
    explorarYBuscarSalida();
  }
}

// Función para medir la distancia con el sensor ultrasónico
float ping() {
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  unsigned long duration = pulseIn(EchoPin, HIGH);
  return float(duration) / 58.2;
}

// Función para explorar y buscar una nueva salida
void explorarYBuscarSalida() {
  float distanciaIzquierda = medirDistancia(anguloMin);
  float distanciaDerecha = medirDistancia(anguloMax);

  servoMotor.write((anguloMin + anguloMax) / 2); // Regresar el servo a la posición central
  delay(500);

  if (distanciaIzquierda > 20.0) {
    moverMotores(HIGH, LOW, LOW, HIGH, 500); // Girar a la izquierda
  } else if (distanciaDerecha > 20.0) {
    moverMotores(LOW, HIGH, HIGH, LOW, 500); // Girar a la derecha
  } else {
    moverMotores(LOW, HIGH, LOW, HIGH, 1000); // Retroceder
  }
}

// Función para mover el servo y medir la distancia
float medirDistancia(int angulo) {
  servoMotor.write(angulo);
  delay(500);
  return ping();
}

// Función para controlar el movimiento de los motores
void moverMotores(int dAdelante, int dAtras, int tAdelante, int tAtras, int tiempo) {
  digitalWrite(motores[0], dAdelante);
  digitalWrite(motores[1], dAtras);
  digitalWrite(motores[2], tAdelante);
  digitalWrite(motores[3], tAtras);
  delay(tiempo);
  detenerMotores();
}

void detenerse(int tiempo) {
  detenerMotores();
  delay(tiempo);
}

// Función para detener todos los motores
void detenerMotores() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(motores[i], LOW);
  }
}
