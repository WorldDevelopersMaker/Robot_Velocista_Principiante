// Sensores TCRT5000 (regleta de 5 sensores)
const int S0 = A0; // Izquierda extrema
const int S1 = A1;
const int S2 = A2; // Central
const int S3 = A3;
const int S4 = A4; // Derecha extrema

// Pines del motor A (izquierdo)
const int AIN1 = 9;
const int AIN2 = 10;
const int PWMA = 5;

// Pines del motor B (derecho)
const int BIN1 = 11;
const int BIN2 = 7;
const int PWMB = 6;

// Pulsador
const int pinPulsador = 2;
bool robotActivo = false;
bool estadoAnteriorPulsador = HIGH;

// LED testigo
const int pinLed = 13;

// Velocidades
int velocidadBase = 180; //255 maximo 
int velocidadGiro = 150;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(S0, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);

  pinMode(pinPulsador, INPUT);
  pinMode(pinLed, OUTPUT);

  detener(); // Estado inicial: detenido
  digitalWrite(pinLed, LOW); // LED apagado
  Serial.begin(9600);
  
}

void loop() {
  // Leer estado del pulsador
  bool estadoActualPulsador = digitalRead(pinPulsador);

 // Detectar flanco de subida (LOW a HIGH) si se usa pull-down
  if (estadoAnteriorPulsador == LOW && estadoActualPulsador == HIGH) {
    robotActivo = !robotActivo;
    delay(300); // Antirebote
  }

  estadoAnteriorPulsador = estadoActualPulsador;

  digitalWrite(pinLed, robotActivo ? HIGH : LOW);

  // Lógica de seguimiento de línea
  if (robotActivo) {
    int s0 = digitalRead(S0);
    int s1 = digitalRead(S1);
    int s2 = digitalRead(S2);
    int s3 = digitalRead(S3);
    int s4 = digitalRead(S4);

    Serial.print(s0); Serial.print(" ");
    Serial.print(s1); Serial.print(" ");
    Serial.print(s2); Serial.print(" ");
    Serial.print(s3); Serial.print(" ");
    Serial.println(s4);

    if (s2 == 0 && s1 == 1 && s3 == 1) {
      avanzar();
    } else if (s1 == 0 || s0 == 0) {
      girarIzquierda();
    } else if (s3 == 0 || s4 == 0) {
      girarDerecha();
    } else {
      detener();
    }
  } else {
    detener();
  }
}

void avanzar() {
  motorA(velocidadBase, false);
  motorB(velocidadBase, false);
}

void girarIzquierda() {
  motorA(velocidadGiro, false);
  motorB(velocidadGiro, true);
}

void girarDerecha() {
  motorA(velocidadGiro, true);
  motorB(velocidadGiro, false);
}

void detener() {
  motorA(0, true);
  motorB(0, true);
}

void motorA(int velocidad, bool adelante) {
  digitalWrite(AIN1, adelante);
  digitalWrite(AIN2, !adelante);
  analogWrite(PWMA, velocidad);
}

void motorB(int velocidad, bool adelante) {
  digitalWrite(BIN1, adelante);
  digitalWrite(BIN2, !adelante);
  analogWrite(PWMB, velocidad);
}
