/*
 * Developed by Denilson Gomes
 * Company: Control Engenharia
 */

#include<SoftwareSerial.h>

int tensaoA;
String aux1;
String aux2;
SoftwareSerial mySerial(3,2);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(A0, INPUT);
  
}

void loop() {
  tensaoA = analogRead(A0);
  if(tensaoA > 800 | true){
    mySerial.println("ATE0");
    delay(2000);
    mySerial.println("AT+CMQTTSTART"); //Establishing MQTT Connection
    delay(2000); 
    mySerial.println("AT+CMQTTACCQ=0,\"Client c\""); //Client ID
    delay(2000);
    mySerial.println("AT+CMQTTCONNECT=0,\"tcp://demo.thingsboard.io:1883\",90,1,\"qAGigVtsUtsWuuCK0q8u\""); //MQTT Server Name for connecting this client
    delay(2000);
    mySerial.println("AT+CMQTTTOPIC=0,23"); //AT Command for Setting up the Publish Topic Name
    delay(1000);
    mySerial.println("v1/devices/me/telemetry"); //AT Command for Setting up the Publish Topic Name
    delay(1000);
    aux1 = "{\"A\":" + String(tensaoA) + "}";
    aux2 = "AT+CMQTTPAYLOAD=0," + String(aux1.length());
    mySerial.println(aux2); //Payload length
    delay(1000);
    mySerial.println(aux1); //Payload message
    delay(1000);
    mySerial.println("AT+CMQTTPUB=0,1,60"); //Acknowledgment
    delay(1000);
    mySerial.println("AT+CMQTTDISC=0,60"); //Disconect
    delay(1000);
    mySerial.println("AT+CMQTTREL=0"); //Release
    delay(1000);
    mySerial.println("AT+CMQTTSTOP"); //Stop
    Serial.println(aux1);
    Serial.println(aux2);
  }
  delay(2000);
}
