//inputs digitais de cada infra
const int INFRA1 = 12;
const int INFRA2 = 11;
const int INFRA3 = 10;
const int INFRA4 = 9;
const int INFRA5 = 8;

//config ponte-h
const int IN1esq = 7; //MOTORES ESQ +
const int IN2esq = 4; //MOTORES ESQ -
const int PWMesq = 6; //VELOCIDADE MOTOR ESQ

const int IN3dir = 3; //MOTORES DIR +
const int IN4dir = 2; //MOTORES DIR -
const int PWMdir = 5; //VELOCIDADE MOTOR DIR

int medPWMesq = 120; //PWM MÉDIO DOS MOTORES LADO ESQ
int medPWMdir = 120; //PWM MÉDIO DOS MOTORES LADO DIR
int PWMesq_val = 0; //PWM NOS MOTORES LADO ESQ
int PWMdir_val = 0; //PWM NOS MOTORES LADO DIR

//valores infra digitais
byte DIGIN1;
byte DIGIN2;
byte DIGIN3;
byte DIGIN4;
byte DIGIN5;

//valor dos erros
int erro;
int erro_anterior;
int erro90dir;
int erro90esq;

//valores p/ PID
double Kp;
double Ki;
double Kd;
double P;
double I;
double D;
double integral = 0;
double PID;

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
    0  0  1  1  1 > erro90dir = 3
    0  0  0  0  1 > erro = 2
    0  0  0  1  0 > erro = 1
    0  0  1  0  0 > erro = 0
    0  1  0  0  0 > erro = -1
    1  0  0  0  0 > erro = -2
    1  1  1  0  0 > erro90esq = -3
  */

  if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 1) && (DIGIN4 == 1) && (DIGIN5 == 1))
  {
    erro90dir = 3;
    erro_anterior = erro90dir;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 0) && (DIGIN4 == 0) && (DIGIN5 == 1))
  {
    erro = 2;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 0) && (DIGIN4 == 1) && (DIGIN5 == 0))
  {
    erro = 1;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 0) && (DIGIN3 == 1) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = 0;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 0) && (DIGIN2 == 1) && (DIGIN3 == 0) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = -1;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 1) && (DIGIN2 == 0) && (DIGIN3 == 0) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro = -2;
    erro_anterior = erro;
  }

  else if ((DIGIN1 == 1) && (DIGIN2 == 1) && (DIGIN3 == 1) && (DIGIN4 == 0) && (DIGIN5 == 0))
  {
    erro90esq = -3;
    erro_anterior = erro90esq;
  }

  //SENTIDO MOTORES
  if (erro90dir == 3) //somente em caso de curvas de 90º p/ dir
  {
    digitalWrite(IN1esq, HIGH);
    digitalWrite(IN2esq, LOW);
    digitalWrite(IN3dir, LOW);
    digitalWrite(IN4dir, HIGH);
    analogWrite(PWMdir, 160);
    analogWrite(PWMesq, 160);
    delay(300);
  }

  else if (erro90esq == -3) //somente em caso de curvas de 90º p/ esq
  {
    digitalWrite(IN1esq, LOW);
    digitalWrite(IN2esq, HIGH);
    digitalWrite(IN3dir, HIGH);
    digitalWrite(IN4dir, LOW);
    analogWrite(PWMdir, 160);
    analogWrite(PWMesq, 160);
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
  Kp = 1;
  Ki = 0.5; 
  Kd = 1;

  //P
  P = Kp * erro;
  Serial.print ("P: ");
  Serial.println (P);

  //I
  if (erro == 0)
    integral = 0;
  integral = integral + erro;
  if (integral > 120)
    integral = 120;
  else if (integral < -120)
    integral = -120;
  I = Ki * (integral + erro);
  Serial.print ("integral: ");
  Serial.println (integral);
  Serial.print ("I: ");
  Serial.println (I);
  
  //D
  D = Kd * (erro - erro_anterior);
   Serial.print ("D: ");
   Serial.println (D);

  //PID
  PID = P + I + D;
  Serial.println(PID); 

  //aplicando nos motores
  if (PID >= 0) //vira p/ direita
  {
    PWMesq_val = medPWMesq;
    PWMdir_val = medPWMdir - PID;
    analogWrite(PWMesq, PWMesq_val);
    analogWrite(PWMdir, PWMdir_val);
    Serial.print ("motor1: ");
    Serial.println (PWMesq_val);
    Serial.print ("motor2: ");
    Serial.println (PWMdir_val);
  }
  
  else //vira p/ esquerda
  {
    PWMesq_val = medPWMesq + PID;
    PWMdir_val = medPWMdir;
    analogWrite(PWMesq, PWMesq_val);
    analogWrite(PWMdir, PWMdir_val);
    Serial.print ("motor1: ");
    Serial.println (PWMesq_val);
    Serial.print ("motor2: ");
    Serial.println (PWMdir_val);
  }
}