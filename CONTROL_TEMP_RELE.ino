//*******************************************//
// CONTROLE DE TEMPERATURA COM RELE          //
//******************************************// 

#include <LiquidCrystal_I2C.h>
//Endereço do LCD
LiquidCrystal_I2C lcd(0x3f, 16, 2);
//Variavel que ira conter o valor lido no Sensor de temperatura
float tempC, SetPoint;
// Carga (pino onde será aplicado o output) --> RELE está ligado ao pino 4.
#define Carga 4    
int power;
const int Botao_DN = 6; // botao para variavel negativa pino 6
const int Botao_UP = 5; // botao para variavel negativa pino 7
int varM = 0, varP = 0; // variaveis de comparacao
int botao_down, botao_up; //variaveis para ler o estado do botao e temperatura




void setup(void)
{
  Serial.begin(9600);
  pinMode(Carga, OUTPUT);
  //Inicializa o LCD
  lcd.init();
  lcd.clear();
  pinMode(Botao_DN, INPUT_PULLUP);
  pinMode(Botao_UP, INPUT_PULLUP);
  SetPoint = 34.0;
}   


// Função que lê a temperatura do sensor LM35 cinco vezes e retorna a média das 5 leituras e calculando a temperatura em °C.
float GetTemp(int sensor)
{
  float temp = 0;
  for(int i=0; i< 5;i++)
  {
    temp += analogRead(sensor);
    delay(100);
  }
  temp = (temp * 0.48875855)/5;
  return (int)temp;
}


void loop()
{
  // Variaveis para leitura dos botoes e ajuste do SetPoint
  botao_down = digitalRead(Botao_DN); //le o estado do botao negativo, apertado ou solto
  botao_up = digitalRead(Botao_UP); //le o estado do botao negativo, apertado ou solto
  // comparacao e subtracao ou adicao de temperatura
  if (botao_down == 1 && varM == 0)
  {
    varM = 1;
  }
  if (botao_down == 0 && varM == 1)
  {
    SetPoint = SetPoint - 1.0;
    varM = 0;
  }
  // funcionamento de comparacao e adicao da temp caso botao = apertado
  if (botao_up == 1 && varP == 0)
  {
    varP = 1;
  }
  if (botao_up == 0 && varP == 1)
  {
    SetPoint = SetPoint + 1.0;
    varP = 0; 
  }
  // Le a informacao do sensor
  tempC = GetTemp(A0);
  // Envia sinal ao RELE para ligar o fogao 
  digitalWrite(Carga, HIGH);
  // Compara a temperatura do sensor com o SetPoint.
  if (tempC >= SetPoint)
  {
    // Envia sinal ao RELE para desligar o fogao 
    digitalWrite(Carga, LOW);
    power = 0;
  }
   if (tempC < SetPoint)
  {
    // Envia sinal ao RELE para ligar o fogao 
    digitalWrite(Carga, HIGH);
    power = 100;
  }
  // Mostra dados no LCD 
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 1);
  lcd.setCursor(0,0);
  lcd.print("Ta=");
  //lcd.setCursor(10,0);
  lcd.print(tempC,0);
  //Simbolo grau
  lcd.write(223);
  lcd.print("C ");
  lcd.print("Ts=");
  lcd.print(SetPoint,0);
  //Simbolo grau
  lcd.write(223);
  lcd.print("C");
  lcd.setCursor(0,1);
  if (power == 0)
  {
    lcd.print("Power=OFF");
  }
  if (power == 100)
  {
    lcd.print("Power=ON ");
  }
  delay(300);
}

