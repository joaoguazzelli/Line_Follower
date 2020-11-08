#include <Arduino.h>


const int motor1 = 3;  // Pino_Velocidade 1º Motor ( 0 a 255) ENA
const int motor2 = 5; //Pino_Velocidade 2º Motor ( 0 a 255) ENB
const int dir1 = 2;  //Pino_Direção do 1º Motor IN1 ponte H; 
const int dir2 = 6; //Pino_Direção do 2º Motor IN3 ponte H;
                
 
#define Sensor1 9
#define Sensor2 11
bool Valor_Sensor1 = 0; 
bool Valor_Sensor2 = 0;
 
void setup()
{

  pinMode(motor1, OUTPUT);   
  pinMode(motor2, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);

  digitalWrite(dir1, HIGH); 
  digitalWrite(dir2, HIGH);

  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
}
 
void loop(){
  //Neste processo armazenamos o valor lido pelo sensor na variável que controla
  // a velocidade dos motores.
  Valor_Sensor1 = digitalRead(Sensor1); 
  Valor_Sensor2 = digitalRead(Sensor2); 
  
   // Aqui criamos nossa condicional que define como o motor se comporta

  if((Valor_Sensor1 == 0) && (Valor_Sensor2 == 0)){ // Se detectar na extremidade das faixas duas cores brancas
    analogWrite(motor1, 150); // Ambos motores ligam na mesma velocidade
    analogWrite(motor2, 150);
  }
  
  if((Valor_Sensor1 == 1) && (Valor_Sensor2 == 0)){ // Se detectar um lado preto e o outro branco
    analogWrite(motor1, 0);  // O motor 1 desliga
    analogWrite(motor2, 150); // O motor 2 fica ligado, fazendo assim o carrinho virar
  }
  
  if((Valor_Sensor1 == 0) && (Valor_Sensor2 == 1)){ // Se detectar um lado branco e o outro preto
    analogWrite(motor1, 150);  //O motor 1 fica ligado
    analogWrite(motor2, 0); // O motor 2 desliga, fazendo assim o carrinho virar no outro sentido
  }  
  
}
