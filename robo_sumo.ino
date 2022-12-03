//bibliotecas
#include <AccelStepper.h> //biblioteca usada para o controle dos motores de passo
#include <Servo.h> //biblioteca utilizada para o controle do servo motor

//setup dos motores de passo na biblioteca de controle AccelStepper
AccelStepper stepper1(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepper2(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

//variaveis
#define sensor_frente A0 //define a entrada A0 como sensor frontal do robo
#define sensor_tras A1 //define a entrada A0 como sensor frontal do robo
#define echoPin 15 //atribui o pino 15 do Arduino ao pino Echo do sensor ultrassonico
#define trigPin 14 //atribui o pino 14 do Arduino ao pino Trigger do sensor ultrassonico

int senf; //variavel que recebera a leitura do sensor frontal
int sent; //variavel que recebera a leitura do sensor traseiro
int alvo0 = 0; //variavel que indica se um alvo foi identificado quando o servo estava a 0 graus
int alvo60 = 0; //variavel que indica se um alvo foi identificado quando o servo estava a 60 graus
int alvo120 = 0; //variavel que indica se um alvo foi identificado quando o servo estava a 120 graus
long duration; // variavel para a duração do tempo de resposta do sensor ultrassonico
int distancia; // variavel para a distancia entre o alvo e o sensor ultrassonico

Servo myservo;  //cria o objeto servo para o controle do servo
int pos = 0;    //variavel para guardar  a posição do servo

void frente()//função de andar para frente
{
  stepper1.moveTo(10000); // função que move os steppers para frente em 10000 passos
  stepper2.moveTo(10000); // função que move os steppers para frente em 10000 passos
}

void tras()//função de andar para tras
{
  stepper1.moveTo(-10000); // função que move os steppers para tras em 10000 passos
  stepper2.moveTo(-10000); // função que move os steppers para tras em 10000 passos
}

void girar()//função de girar 180 graus
{
  stepper1.moveTo(100); // função que gira os steppers em 180 graus
  stepper2.moveTo(-100); // função que gira os steppers em 180 graus
}

void para() //função de andar parar e zerar a posição dos motores de passo
{
  stepper1.stop(); // função que para os steppers 
  stepper2.stop(); // função que para os steppers 
  stepper1.setCurrentPosition(0); // função que atribui a posição dos steppers a 0
  stepper2.setCurrentPosition(0); // função que atribui a posição dos steppers a 0
}
void varredura0() //função responsavel pela varredura no angulo 0
{
  myservo.write(0); //manda o servo para a posição de 0 graus
  pos = 0; //indica que a posição atual do servo é 0 graus
}

void varredura60() //função responsavel pela varredura no angulo 60
{
  myservo.write(60); //manda o servo para a posição de 60 graus
  pos = 60; //indica que a posição atual do servo é 60 graus
}

void varredura120() //função responsavel pela varredura no angulo 120
{
  myservo.write(120); //manda o servo para a posição de 120 graus
  pos = 120; //indica que a posição atual do servo é 120 graus
}

void setup() {
  Serial.begin(9600);
  myservo.attach(12); // atribui o servo motor ao pino 12
  // Configuração de cada stepper
  stepper1.setMaxSpeed(100); // Configuração de velocidade máxima do stepper 1
  stepper1.setAcceleration(100); // Configuração de aceleração máxima do stepper 1
  stepper2.setMaxSpeed(100); // Configuração de velocidade máxima do stepper 2
  stepper2.setAcceleration(100); // Configuração de aceleração máxima do stepper 2
//pinagem dos sensores
  pinMode(sensor_frente, INPUT);
  pinMode(sensor_tras, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myservo.write(0); // coloca a posição inicial do servo motor para 0 graus

}

void loop() {

//variaveis recebem a leitura do valor dos sensores(1 ou 0)
  sent = digitalRead(sensor_tras);
  senf = digitalRead(sensor_frente);

  //Serial.print(senf);
  //Serial.println(sent);

  if (!senf && !sent && alvo0 == 0 && alvo60 == 0 && alvo120 == 0) //segue em frente caso nenhum dos dois sensores infravermelhos estajam ativos
  {
    frente();
    stepper1.run(); //função que mantem o funcionamento dos steppers até que todos os passos sejam dados
    stepper2.run(); //função que mantem o funcionamento dos steppers até que todos os passos sejam dados
  }

  else if (senf && !sent && alvo0 == 0 && alvo60 == 0 && alvo120 == 0) //da meia volta quando chega na borda da arena
  {
    girar();
    if(stepper1.currentPosition() == 100 && stepper2.currentPosition() == -100)
    {
      para();
      frente();
    }
    stepper1.run();
    stepper2.run();
  }

  else if (!senf && !sent && pos == 120 && alvo0 == 0 && alvo60 == 0 && alvo120 == 0) //faz a varredura no grau 0 
  {
    delay(1500);
    varredura0();
  }

  else if (!senf && !sent && pos == 0 && alvo0 == 0 && alvo60 == 0 && alvo120 == 0) //faz a varredura no grau 60 
  {
    delay(1500);
    varredura60();
  }

  else if (!senf && !sent && pos == 60 && alvo0 == 0 && alvo60 == 0 && alvo120 == 0) //faz a varredura no grau 120 
  {
    delay(1500);
    varredura120();
  }
  if (pos == 0 && distancia > 1)//se o servo estiver na posição 0 e identificar um alvo
  {
    alvo0 = 1; //identifica atraves da variavel alvo0
    alvo60 = 0;
    alvo120 = 0;
    if(alvo0)//gira o robo na direção dos 0 graus
    {
      para();
      girar();
      if(stepper1.currentPosition() == 100 && stepper2.currentPosition() == -100)//quando o robo chega la
    {
      frente();//segue em frente
    }
    }
    stepper1.run();
    stepper2.run();
    
  }
  if (pos == 60 && distancia > 1)//se o servo estiver na posição 60 e identificar um alvo
  {
    alvo0 = 0;
    alvo60 = 1; //identifica atraves da variavel alvo60
    alvo120 = 0;
    if(alvo0)
    {
      para();
      girar(); //gira o robo na direção dos 60 graus
      if(stepper1.currentPosition() == 100 && stepper2.currentPosition() == -100)
    {
      frente(); //segue em frente
    }
    }
    stepper1.run();
    stepper2.run();
  }
  if(pos == 120 && distancia > 1)//se o servo estiver na posição 120 e identificar um alvo
  {
    alvo0 = 0;
    alvo60 = 0;
    alvo120 = 1; //identifica atraves da variavel alvo120
    if(alvo0)
    {
      para();
      girar(); //gira o robo na direção dos 120 graus
      if(stepper1.currentPosition() == 100 && stepper2.currentPosition() == -100)
    {
      frente(); //segue em frente
    }
    }
    stepper1.run();
    stepper2.run();
    
  }
}


