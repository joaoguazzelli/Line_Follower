/* 
Etec Presidente Vargas - Outubro de 2020
Algoritmo PID para controle de um robô seguidor de linha.
Autores:
    - João Cancer Nº? Turma 1
    - Jojo Sobrenome Nº? Turma 1
    - Lívia Fonseca Nº? Turma 2
    - Vinícius Kin Nº? Turma 2
*/

#include <Arduino.h>

//=================================================
//inputs digitais dos sensores de infravermelho
const int sensores[5] = [8, 9, 10, 11, 12];

//config ponte-h
const int 

const int IN1 = 2; //MOTORES ESQ +
const int IN2 = 4; //MOTORES ESQ -
const int enA = 3; //VELOCIDADE MOTOR ESQ

const int IN3 = 6; //MOTORES DIR +
const int IN4 = 7; //MOTORES DIR -
const int enB = 5; //VELOCIDADE MOTOR DIR

int medPWMesq = 160; //PWM MÉDIO DOS MOTORES LADO ESQ
int medPWMdir = 160; //PWM MÉDIO DOS MOTORES LADO DIR
int PWMesq_val = 0; //PWM NOS MOTORES LADO ESQ
int PWMdir_val = 0; //PWM NOS MOTORES LADO DIR

byte D[5] = [0, 0, 0, 0, 0];

//valor dos erros
int erro = 0;
int erro_anterior = 0;
int erro90dir = 0;
int erro90esq = 0;

//valores para o PID
int Kp = 0;
int Ki = 0;
int Kd = 0;
int P = 0;
int I = 0;
int D = 0;
int integral = 0;
int PID = 0;

void setup()
{
  for(int i = 0; i < 5; i++)
    pinMode(sensores[i], INPUT);

  pinMode(IN1esq, OUTPUT);
  pinMode(IN2esq, OUTPUT);
  pinMode(PWMesq, OUTPUT);

  pinMode(IN3dir, OUTPUT);
  pinMode(IN4dir, OUTPUT);
  pinMode(PWMdir, OUTPUT);
}

void loop()
{

}

void calculaErro()
{
    //leitura infras
    DIGIN1 = digitalRead(INFRA1);
    DIGIN2 = digitalRead(INFRA2);
    DIGIN3 = digitalRead(INFRA3);
    DIGIN4 = digitalRead(INFRA4);
    DIGIN5 = digitalRead(INFRA5);

    /*determinar erro
    ───────TABELA────────────
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

}


void 