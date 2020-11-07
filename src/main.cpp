#include <Arduino.h>

/* PINOS ARDUINO:
  00 - open
  01 - open
  02 - IN1esq
  03 - PWMesq
  04 - IN2esq
  05 - PWMdir
  06 - IN3dir
  07 - IN4dir
  08 - INFRA5
  09 - INFRA4
  10 - INFRA3
  11 - INFRA2
  12 - INFRA1
  13 - open        */

//inputs digitais de cada infra
const int INFRA1 = 12;
const int INFRA2 = 11;
const int INFRA3 = 10;
const int INFRA4 = 9;
const int INFRA5 = 8;

//config ponte-h
const int IN1esq = 2; //MOTORES ESQ +
const int IN2esq = 4; //MOTORES ESQ -
const int PWMesq = 3; //VELOCIDADE MOTOR ESQ

const int IN3dir = 6; //MOTORES DIR +
const int IN4dir = 7; //MOTORES DIR -
const int PWMdir = 5; //VELOCIDADE MOTOR DIR

int medPWMesq = 160; //PWM MÉDIO DOS MOTORES LADO ESQ
int medPWMdir = 160; //PWM MÉDIO DOS MOTORES LADO DIR
int PWMesq_val = 0; //PWM NOS MOTORES LADO ESQ
int PWMdir_val = 0; //PWM NOS MOTORES LADO DIR

//valores infra digitais
byte DIGIN1 = 0;
byte DIGIN2 = 0;
byte DIGIN3 = 0;
byte DIGIN4 = 0;
byte DIGIN5 = 0;

//valor dos erros
int erro = 0;
int erro_anterior = 0;
int erro90dir = 0;
int erro90esq = 0;

//valores p/ PID
int Kp;
int Ki;
int Kd;
int P;
int I;
int D;
int integral;
int PID;


void setup()
{
  Serial.begin(9600);
  pinMode(INFRA1, INPUT);
  pinMode(INFRA2, INPUT);
  pinMode(INFRA3, INPUT);
  pinMode(INFRA4, INPUT);
  pinMode(INFRA5, INPUT);
  pinMode(INFRA1, INPUT);

  pinMode(IN1esq, OUTPUT);
  pinMode(IN2esq, OUTPUT);
  pinMode(PWMesq, OUTPUT);

  pinMode(IN3dir, OUTPUT);
  pinMode(IN4dir, OUTPUT);
  pinMode(PWMdir, OUTPUT);
}

void loop()
{
  //leitura infras
  DIGIN1 = digitalRead(INFRA1);
  DIGIN2 = digitalRead(INFRA2);
  DIGIN3 = digitalRead(INFRA3);
  DIGIN4 = digitalRead(INFRA4);
  DIGIN5 = digitalRead(INFRA5);

  /*determinar erro
    ~~~~TABELA~~~~
    S1-S2-S3-S4-S5
    0  0  1  1  1 > erro = 5
    0  0  0  0  1 > erro = 4
    0  0  0  1  1 > erro = 3
    0  0  0  1  0 > erro = 2
    0  0  1  1  0 > erro = 1
    0  0  1  0  0 > erro = 0
    0  1  1  0  0 > erro = -1
    0  1  0  0  0 > erro = -2
    1  1  0  0  0 > erro = -3
    1  0  0  0  0 > erro = -4
    1  1  1  0  0 > erro = -5
  */

  if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 1) && (DIGIN4 == 1) && (DIGIN5 == 1))
  {
    erro90dir = 5;
    erro_anterior = erro90dir;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 0) && (DIGIN4 == 0) && (DIGIN5 == 1))
  {
    erro = 4;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 0) && (DIGIN4 == 1) && (DIGIN5 == 1))
  {
    erro = 3;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 0) && (DIGIN4 == 1) && (DIGIN5 == 0))
  {
    erro = 2;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 1) && (DIGIN4 == 1) && (DIGIN5 == 0))
  {
    erro = 1;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 1) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = 0;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 1) && (DIGIN3 == 1) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = -1;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 1) && (DIGIN3 == 0) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = -2;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 1) && (DIGIN2 == 1) && (DIGIN3 == 0) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = -3;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 1) && (DIGIN2 == 0) && (DIGIN3 == 0) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = -4;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 1) && (DIGIN2 == 1) && (DIGIN3 == 1) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro90esq = -5;
    erro_anterior = erro90esq;
  }

  //SENTIDO MOTORES
  if (erro90dir == 5) //somente em caso de curvas de 90º p/ dir
  {
    digitalWrite(IN1esq, HIGH);
    digitalWrite(IN2esq, LOW);
    analogWrite(PWMesq, 255);
    
    digitalWrite(IN3dir, LOW);
    digitalWrite(IN4dir, HIGH);
    analogWrite(PWMdir, 255);
    delay(300);
  }

  else if (erro == -5) //somente em caso de curvas de 90º p/ esq
  {
    digitalWrite(IN1esq, LOW);
    digitalWrite(IN2esq, HIGH);
    digitalWrite(IN3dir, LOW);
    digitalWrite(IN4dir, HIGH);
    analogWrite(PWMdir, 255);
    analogWrite(PWMesq, 255);
    delay(300);
  }

  else //em todas as outras curvas
  {
    digitalWrite(IN1esq, HIGH);
    digitalWrite(IN2esq, LOW);
    digitalWrite(IN3dir, HIGH);
    digitalWrite(IN4dir, LOW);
  }

  //ALGORITMO PID ATRAVÉS DE ZIEGLER-NICHOLS
  //valores
  Kp = 36;
  Ki = 18;
  Kd = 18;

  //P
  P = Kp * erro;

  //I
  integral = integral + erro;
  if (integral > 255)
    integral = 255;
  else if (integral < -255)
    integral = -255;
  I = Ki * (integral + erro);

  //D
  D = Kd * (erro - erro_anterior);

  //PID
  PID = P + I + D;

  //aplicando nos motores
  if (PID >= 0) //vira p/ direita
  {
    PWMesq_val = medPWMesq;
    PWMdir_val = medPWMdir - PID;
    analogWrite(PWMesq, PWMesq_val);
    analogWrite(PWMdir, PWMdir_val);
  }
  else //vira p/ esquerda
  {
    PWMesq_val = medPWMesq + PID;
    PWMdir_val = medPWMdir;
    analogWrite(PWMesq, PWMesq_val);
    analogWrite(PWMdir, PWMdir_val);
  }

}