#include <Arduino.h>

//protótipo das funções
void calculaPID();
void calculaErro();
void leituraInfras();
void acionaMotores();

//Definição dos pinos dos sensores IR
#define INFRA1 12
#define INFRA2 11
#define INFRA3 10
#define INFRA4 9
#define INFRA5 8

int DIGIN1 = 0, DIGIN2 = 0, DIGIN3 = 0, DIGIN4 = 0, DIGIN5 = 0;

//Definição dos pinos da ponte H
#define IN1 7
#define IN2 4
#define ENA 6

#define IN3 3
#define IN4 2
#define ENB 5

#define maxSpeed 120
#define baseSpeed 100

int PWMesq = 0, PWMdir = 0;

//Definição das Variaveis PID
float KP = 0;
float KI = 0;
float KD = 0;
float P = 0, I = 0, D = 0, PID = 0;

//erros
int erro = 0, erroAnterior = 0, erro90esq = 0, erro90dir = 0;

void setup(){
    Serial.begin(9600);

    pinMode(INFRA1, INPUT);
    pinMode(INFRA2, INPUT);
    pinMode(INFRA3, INPUT);
    pinMode(INFRA4, INPUT);
    pinMode(INFRA5, INPUT);
    pinMode(INFRA1, INPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
}


void loop(){
    Serial.println("--------------------------");
    
    calculaErro();
    calculaPID();
    acionaMotores();
    
    Serial.println("--------------------------");


}


void leituraInfras(){
    DIGIN1 = digitalRead(INFRA1);
    DIGIN2 = digitalRead(INFRA2);
    DIGIN3 = digitalRead(INFRA3);
    DIGIN4 = digitalRead(INFRA4);
    DIGIN5 = digitalRead(INFRA5);
}


void calculaErro(){
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

    leituraInfras();

    if(DIGIN1 == 0 && DIGIN2 == 0 && DIGIN3 == 1 && DIGIN4 == 1 && DIGIN5 == 1)
        erro90dir = 3;

    else if(DIGIN1 == 0 && DIGIN2 == 0 && DIGIN3 == 0 && DIGIN4 == 0 && DIGIN5 == 1)
        erro = 2;
    
    else if(DIGIN1 == 0 && DIGIN2 == 0 && DIGIN3 == 0 && DIGIN4 == 1 && DIGIN5 == 0)
        erro = 1;

    else if(DIGIN1 == 0 && DIGIN2 == 0 && DIGIN3 == 1 && DIGIN4 == 0 && DIGIN5 == 0)
        erro = 0;


    else if(DIGIN1 == 0 && DIGIN2 == 1 && DIGIN3 == 0 && DIGIN4 == 0 && DIGIN5 == 0)
        erro = -1;

    else if (DIGIN1 == 1 && DIGIN2 == 0 && DIGIN3 == 0 && DIGIN4 == 0 && DIGIN5 == 0)
        erro = -2;

    else if(DIGIN1 == 1 && DIGIN2 == 1 && DIGIN3 == 1 && DIGIN4 == 0 && DIGIN5 == 0)
        erro90esq = -3;

    Serial.print("Erro: ");
    Serial.println(erro);
}


void calculaPID(){
    if(erro == 0)
        I = 0;

    P = erro;

    I = I + erro;
    if(I > maxSpeed)
        I = maxSpeed;
    else if(I < -maxSpeed)
    I = -maxSpeed;

    D = erro - erroAnterior;
    
    PID = ((KP * P) + (KI * I) + (KD * D));
    erroAnterior = erro;

    Serial.print("PID: ");
    Serial.println(PID);

    if(PID >= 0){
        PWMesq = baseSpeed;
        PWMdir = baseSpeed - PID;
    }

    else
    {
        PWMesq = baseSpeed + PID;
        PWMdir = baseSpeed;
    }

    Serial.print("Motor Direita: ");
    Serial.println(PWMdir);
    Serial.print("Motor Esquerda: ");
    Serial.println(PWMesq);

}


void acionaMotores(){
    if(erro90dir == 3){
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, baseSpeed);

        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH); 
        analogWrite(ENB, baseSpeed);
        delay(300);
        
        erro90dir = 0;
    }

    else if(erro90esq == -3){
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, baseSpeed);

        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, baseSpeed);
        delay(300);

        erro90esq = 0;
    }

    else{
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, PWMesq);

        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, PWMdir);
        delay(300);
    }
}



