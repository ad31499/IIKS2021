#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <stdlib.h>

   
  EthernetClient ethClient;
  PubSubClient mqttClient(ethClient);
   
 
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Nastavi mac za arduino
  IPAddress ip(192, 168, 1, 33); //Nastavi ip za arduino
   
  
  const char* server = "192.168.1.160"; //Definiraj ip serverja (mqtt broker)


     
void setup() {

  Serial.begin(9600); 
  
  Ethernet.begin(mac, ip); //Vzpostavi ethernet       

  delay(3000); //Počakaj nekaj časa, da se ethernet postavi
  
  mqttClient.setServer(server, 1883); //Pripravi server za povezavo 

  const char password[] = "arduino123";
  const char username[] = "arduino";
  
  if (mqttClient.connect("anze1pc", username, password)) //Poveži se na server z id-jom anze1pc 
                                                         //in podaj se username in password
  { 
    Serial.println("Povezava z MQTT brokerjem je vzpostavljena");
  } 
  else 
  {
    Serial.println("Povezava je prekinjena");
  }
}

void loop() {
  static unsigned long previousTime = millis();
  unsigned long currentTime = millis();
  if ((currentTime/1000) - (previousTime/1000) > 3600){ //Na vsako uro preberi podatek in pošlji
    previousTime = currentTime;
   
  float sensorValue = analogRead(A1); //Preberi pin A1
 
  float temp = ((5 * sensorValue / 1023) - 0.5) / 10; //Izračunaj temperaturo
  temp = temp * 1000; //Dokončno izračunaj

  char buffer[6];
  dtostrf(temp, 4, 2, buffer); //V buffer shrani vrednost
 
  if(mqttClient.publish("senzor/temp", buffer)) //Publish temperaturo pod topic senzor/temp
  {
    Serial.println(buffer); //Če je prenos uspel, v serial monitor izpiši prenešen podatek
  }
  else
  {
    Serial.println("Sporočilo se ni preneslo!!");
  }
 
  }
}
