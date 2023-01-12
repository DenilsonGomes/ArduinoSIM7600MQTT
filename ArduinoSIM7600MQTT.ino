/*
* Developed by Denilson Gomes, Aluisio Cavalcante
* Company: Control Engenharia
* 
* Codigo para: 
* Inscrever em topico de servidor MQTT
* Publicar payload no topico
* Confirmar publicação ao receber payload publicada no topico inscrito
*/
// -- Bibliotecas
#include "Arduino.h"
#include "GSMSim.h"
#include "stdint.h"
#include "SoftwareSerial.h" // Biblioteca SoftwareSerial

// -- Variaveis
unsigned long pisca_sinal_aceso;
String host = "tcp://demo.thingsboard.io:1883";
String token = "RtSOj8NE3rxyFP1wFPLH";
String topico = "v1/devices/me/telemetry";
String payload = "{\"Fal\":\"A\",\"Bat\":12.78,\"Sig\":100}";
String aux;
String a;
int tries;
#define MAXTRIES 50

// -- Objetos
SoftwareSerial mySerial(2, 4); // Objeto mySerial nos pinos 2 e 3
GSMSim modemSIM(mySerial, 40);

int modem(String snd, String rcv, uint16_t tempo)
{
  uint16_t ticks_for = tempo/25;
  Serial.print("Comando: ");Serial.println(snd);
  mySerial.println(snd);
  for (uint16_t m = 0; m < ticks_for; m++)
  {
//    wdt_reset();
    delay(25);
    if (mySerial.available())
    {
      delay(50);
      a = mySerial.readString();
      //Serial.println(a);  //Print the command response
      
      if (a.indexOf(rcv) > -1 || rcv.length() == 0)
      {
        Serial.print("Retorno do comando: ");Serial.println(a);
        return 0;
      }
    }
  }
  return 1;
}

void serialCommand(String command) {
  Serial.print("Command: ");Serial.println(command);
  mySerial.println(command);

  char dat;
  while (mySerial.available()) {
    dat = mySerial.read();
    Serial.print(dat);
    delay(10);
  }
  Serial.println(dat);  //Print the command response
}

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando modem.");
  delay(20000);
  mySerial.begin(9600);
  Serial.println(modem("AT+CRESET", "OK", 60000));
  delay(20000);
  //modem("AT+CNMP=2", "OK",60000);
}

void loop() {
  Serial.print("RSSI: ");
  pisca_sinal_aceso = modemSIM.signalQuality() * 3.25;
  Serial.println(pisca_sinal_aceso);

  modem("AT+CPIN?", "OK",30000);
  modem("AT+CSQ", "OK",30000);
  modem("AT+CREG?", "OK", 30000);
  modem("AT+COPS?", "OK",30000);
  modem("AT+CNMP?", "OK",30000);
  modem("AT+CNSMOD?", "OK",30000);

  // Inicia MQTT
  while(modem("AT+CMQTTSTART", "RT:", 120000) && tries <= MAXTRIES){//30000ms //VERIFICAR SE O RETORNO É :0 OU : 0  
    Serial.print(tries);
    Serial.println(" tentativa CMQTTSTART");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTSTART");
    //Ação
  }

  // Inicia cliente MQTT
  tries=0;
  while(modem("AT+CMQTTACCQ=0,\"test\",0,4", "OK", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa CMQTTACCQ");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTACCQ");
    //Ação
  }

  // Conecta ao servidor MQTT
  tries=0;
  //aux = "AT+CMQTTCONNECT=0,\"" + host + "\",90,1,\"" + token + "\"";
  //while(modem(aux, "0,0", 150000) && tries <= MAXTRIES){
  while(modem("AT+CMQTTCONNECT=0,\"tcp://demo.thingsboard.io:1883\",300,1,\"RtSOj8NE3rxyFP1wFPLH\"", "0,0", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa CMQTTCONNECT");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTCONNECT");
    //Ação
  }

// Seta o topico de inscrição
  tries=0;
  aux = "AT+CMQTTSUBTOPIC=0," + String(topico.length()) + ",2";
  Serial.println(aux);
  serialCommand(aux);  //Length of the topic string
  delay(20);
  while(modem(topico, "OK", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa CMQTTSUBTOPIC");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTSUBTOPIC");
    //Ação
  }

  // Inscreve no topico
  while(modem("AT+CMQTTSUB=0", "OK", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa CMQTTSUB");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTSUB");
    //Ação
  }

  // Seta topico de publicação
  aux = "AT+CMQTTTOPIC=0," + String(topico.length());
//  Serial.println(aux);
  serialCommand(aux);  //Length of the topic string
  delay(20);
  
  tries=0;
  while(modem(topico, "OK", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa de setar o topico");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro ao setar o topico");
    //Ação
  }

  // Seta payload
  aux = "AT+CMQTTPAYLOAD=0," + String(payload.length());
//  Serial.println(aux);
  serialCommand(aux);  //Length of the payload
  delay(20);
  tries=0;
  while(modem(payload, "OK", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa de setar a payload");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro ao setar a payload");
    //Ação
  }

  // Publica
  tries=0;
  while(modem("AT+CMQTTPUB=0,1,60", "0,0", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa de publicar a payload");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro ao publicar a payload");
    //Ação
  }
  
  // Ler retorno da mySerial e testar com payload enviada
  int qwe=1;
  for (uint16_t m = 0; m < 9600; m++)
  {
//    wdt_reset();
    delay(25);
    if (mySerial.available())
    {
      delay(50);
      a = mySerial.readString();
      Serial.println("Printando publicação recebida");  //Print the command response
      Serial.println(a);  //Print the command response
      
      if (a.indexOf(payload) > -1 || payload.length() == 0)
      {
        qwe=0;
        Serial.println("Publicação confirmada!");
        Serial.print("Retorno: ");Serial.println(a);
      }
    }
  }
  if(qwe){
   Serial.println("Tempo esgotado sem confirmação de publicação"); 
  }

  // Disconecta
  tries=0;
  while(modem("AT+CMQTTDISC=0,60", "0,0", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa de CMQTTDISC");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTDISC");
    //Ação
  }

  // Release
  tries=0;
  while(modem("AT+CMQTTREL=0", "OK", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa de CMQTTREL");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTREL");
    //Ação
  }

  // Stop
  tries=0;
  while(modem("AT+CMQTTSTOP", "OP:", 120000) && tries <= MAXTRIES){
    Serial.print(tries);
    Serial.println(" tentativa de CMQTTSTOP");
    tries++;
  }
  if(tries >= MAXTRIES){
    Serial.println("Erro CMQTTSTOP");
    //Ação
  }
  delay(300000);
}
