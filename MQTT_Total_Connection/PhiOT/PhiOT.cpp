
#include "Arduino.h"
#include "PhiOT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>


int indicatingLed=LED_BUILTIN;
bool NetworkConnected = false;

//Wifi Clients Configs
IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

//No of seconds system will be busy trying to figure out if it is connected to network or not.
int maxTimeOut = 20;

//Name of the access point to be created.
const char *AccessPointName = "PhiOTConnect";

ESP8266WebServer server(80);

//Mqtt configs
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

void PhiOT::Initialize() {

  Serial.println("Checking if connection already availabe.");
  PhiOT::CheckingConnectionStatusWithDelay();
  
}

void PhiOT::CheckingConnectionStatusWithDelay()
{
  Serial.println("Checking Connection Status With Delay.");
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    count++;
    delay(1000);
    Serial.print(".");

    if (count == maxTimeOut) 
    {
      Serial.println("");
      Serial.println("Could not connect to wifi,Ending connection loop.");
      Serial.println("Starting up access point.");
      NetworkConnected = false;
      PhiOT::setAccessPoint();
      return;
    }
  }

  NetworkConnected = true;
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connection Successful");

  PhiOT::SwitchOffAccessPoint();

  //initializing mqtt
  PhiOT::mqttInit();
}

void PhiOT::SwitchOffAccessPoint()
{
  Serial.println("Switching off Access Point.");
  WiFi.softAPdisconnect(false);
  WiFi.enableAP(false);
}

void PhiOT::setAccessPoint()
{
  Serial.print("Configuring access point...");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(AccessPointName);
  
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  PhiOT::ServerRouters();

  server.begin();
  Serial.println("HTTP server started");

  PhiOT::indicatorForAccessPoint();
}

void PhiOT::ServerRouters()
{
  server.on("/wificonnect", [this]() {

    server.send(200, "text/plain", server.arg("ssid")+"\n"+server.arg("password"));
    
    PhiOT::WifiConnectionSetup(server.arg("ssid"), server.arg("password"));

  });
  //server.on("/checkstatus", checkConnectionStatus);
  //server.on("/wifiscan", wifiScan);
}

void PhiOT::WifiConnectionSetup(String ssid, String password) 
{
  Serial.println("Connecting to ");
  Serial.println(ssid);
  Serial.println(password);

  WiFi.begin(ssid.c_str(), password.c_str());

  PhiOT::CheckingConnectionStatusWithDelay();
}

void PhiOT::mqttInit()
{
  client.setServer(mqtt_server, 1883);
  //the callback function
  client.setCallback([this](char* topic, byte* payload, unsigned int length) {
  	  	PhiOT::callback(topic, payload, length);
        PhiOT::lightIndicatorConfirmation();
	});
}

void PhiOT::callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void PhiOT::reconnect() 
{

  int count = 0;
  while (!client.connected()) 
  {
    Serial.println("Attempting MQTT connection... With Delay");
    String clientId = _token;
    
    //checking if connection successful
    if (client.connect(clientId.c_str())) 
    {

      Serial.println("Connected and publishing status to server");
      client.publish(publishTopic.c_str(), clientId.c_str());
      client.subscribe(subscribeTopic.c_str());
      PhiOT::lightIndicatorConfirmation();
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      count++;
      // Wait 5 seconds before retrying
      delay(5000);
      if (count == maxTimeOut/5) 
      {
        Serial.println("");
        Serial.println("Could not connect to Mqtt server,Ending Mqtt loop.");
        Serial.println("Starting up access point.");
        PhiOT::setAccessPoint();
        return;
      }
    }
  }

}

void PhiOT::phiLoop()
{
  //delay(1000);
  server.handleClient();

  if(NetworkConnected)
  {
    if (!client.connected()) 
    {
      Serial.println("Mqtt connection initializing..");
      PhiOT::reconnect();
    }
    client.loop();
  }

	
  
}

void PhiOT::lightIndicatorConfirmation()
{
   for(int i=0;i<=5;i++)
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

void PhiOT::indicatorForAccessPoint()
{
  digitalWrite(indicatingLed,0);
}
