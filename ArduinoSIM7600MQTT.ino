/*
 * Developed by Denilson Gomes
 * Computer engineer
 * Company: Control Engenharia
 */

// Bibliotecas
#include <SoftwareSerial.h> // Biblioteca SoftwareSerial

// Variáveis
int tensaoA; // Variavel para leitura analógica
String aux1; // String auxiliar1
String aux2; // String auxiliar2

// Objeto
SoftwareSerial mySerial(2,3); // Objeto mySerial

// Setup
void setup() {
  Serial.begin(9600); // Inicia a serial
  mySerial.begin(9600); // Inicia a mySerial
  pinMode(A0, INPUT); // Sinal analógico na porta A0
}

void loop() {
  tensaoA = analogRead(A0); // Leitura da tensão A
  if(1){ // Caso seja necessário enviar a leitura por MQTT
    Serial.println("Entrou no if");
    mySerial.println("ATE0"); // Inicia o modem
    delay(3000);
    mySerial.println("AT+CMQTTSTART"); // Inicia conexão MQTT
    delay(3000); 
    mySerial.println("AT+CMQTTACCQ=0,\"Client c\""); // Client ID
    delay(3000);
    mySerial.println("AT+CMQTTCONNECT=0,\"tcp://demo.thingsboard.io:1883\",90,1,\"EKx0Da1Zjiue6N9XkVLf\""); // Conecta ao Servidor MQTT
    delay(3000);
    mySerial.println("AT+CMQTTTOPIC=0,23"); // Tamanho do nome do topico
    delay(3000);
    mySerial.println("v1/devices/me/telemetry"); // Topico
    delay(3000);
    aux1 = "{\"B\":" + String(tensaoA) + "}"; // payload
    aux2 = "AT+CMQTTPAYLOAD=0," + String(aux1.length());
    mySerial.println(aux2); // Define tamanho da payload
    delay(3000);
    mySerial.println(aux1); // Payload
    delay(3000);
    mySerial.println("AT+CMQTTPUB=0,1,60"); // Publica a payload
    delay(3000);
    mySerial.println("AT+CMQTTDISC=0,60"); // Disconecta o cliente
    delay(3000);
    mySerial.println("AT+CMQTTREL=0"); // Release o servidor
    delay(3000);
    mySerial.println("AT+CMQTTSTOP"); // Encerra conexão MQTT
    Serial.println(aux2);
    Serial.println(aux1);
  }
//  Serial.println(tensaoA);
  delay(10000);
}
