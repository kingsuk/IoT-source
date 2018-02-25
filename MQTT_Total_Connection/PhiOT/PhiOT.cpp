
#include "Arduino.h"
#include "PhiOT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


int indicatingLed=2;

const char* ssid = "iBall-Baton";
const char* password = "kingsukmajumder2";
const char* mqtt_server = "139.59.28.88";

String token = "";
String publishTopic = "";
String subscribeTopic = "";

WiFiClient espClient;
PubSubClient client(espClient);

PhiOT::PhiOT(String token)
{
  _token = token;
  publishTopic = "outTopic/"+token;
  subscribeTopic = "inTopic/"+token;
  
  pinMode(indicatingLed, OUTPUT);
}

void PhiOT::setup_wifi() {

   delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, 1883);
  
  //the callback function
  client.setCallback([this](char* topic, byte* payload, unsigned int length) {
  	
  	  	PhiOT::callback(topic, payload, length);
		
		
	});
  
}

void PhiOT::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void PhiOT::reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = _token;
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      client.publish(publishTopic.c_str(), clientId.c_str());
      
      client.subscribe(subscribeTopic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  
  digitalWrite(indicatingLed,0);
  delay(3000); 
  digitalWrite(indicatingLed,1);
}

void PhiOT::phiLoop()
{
	if (!client.connected()) {
		Serial.println("Mqtt connection initializing..");
	    PhiOT::reconnect();
	  }
	  client.loop();
}





void PhiOT::lightIndicator()
{
   for(int i=0;i<=10;i++)
    {
      if(i%2==0)
      {
        digitalWrite(indicatingLed,0);
      }
      else
      {
        digitalWrite(indicatingLed,1);
      }
      delay(200);
    } 
      
}
