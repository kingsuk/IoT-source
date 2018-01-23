/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
// Update these with values suitable for your network.
#include <ArduinoJson.h>

const char* mqtt_server = "139.59.32.194";
String topicName = "inTopic";
String outTopicName = "outTopic";
String device_id = "1002";
String topicString = topicName+"/"+device_id;
String outTopicString = outTopicName+"/"+device_id;
const char* inTopic = topicString.c_str();
const char* outTopic = outTopicString.c_str();

//int led = 2;



WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int indicatingLed = LED_BUILTIN;

WiFiManager wifiManager;

void setup_wifi() {
    

    
    
    wifiManager.autoConnect("ESPController");
    //indicating wifi setup begin
    

    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    
    lightIndicator();
    
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char JSONMessage[80];
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    JSONMessage[i] = (char)payload[i];
  }

  //publishing back whenever something received
  client.publish(outTopic, "Online");
  
  Serial.println("Initial string value: ");
  Serial.println(JSONMessage);

  StaticJsonBuffer<300> JSONBuffer;   //Memory pool
  JsonObject& parsed = JSONBuffer.parseObject(JSONMessage); //Parse message
 
  if (!parsed.success()) {   //Check for errors in parsing
 
    Serial.println("Parsing failed");
    delay(5000);
    return;
 
  }

  String header = parsed["header"];
  Serial.println(header);
  if(header=="data")
  {
    String data = parsed["data"];
    Serial.println(data);
    
    JsonObject& parsedData = JSONBuffer.parseObject(data); //Parse message

     for (JsonObject::iterator i=parsedData.begin(); i!=parsedData.end(); ++i)
     {
        const char * key = i->key;
        Serial.println(key);
        const char * value = parsedData[key];
        int valInt = atoi(value);
        int keyInt = atoi(key);
    
       
    
        pinMode(keyInt, OUTPUT);
        digitalWrite(keyInt,valInt);
        
        
        Serial.println(valInt);
    }
    
  }
  else if(header=="station")
  {
    lightIndicator();
    digitalWrite(indicatingLed,0);
    if (!wifiManager.startConfigPortal("ESPController")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    lightIndicator();
  }
  





}

void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      lightIndicator();
      // Once connected, publish an announcement...
      client.publish(outTopic, "Online");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
//  pinMode(led, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(indicatingLed, OUTPUT);
  pinMode(indicatingLed, 1);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

      
      if (!client.connected()) {
      reconnect();
      }
      client.loop();
       
    
//      long now = millis();
//      if (now - lastMsg > 2000) {
//        lastMsg = now;
//        ++value;
//        snprintf (msg, 75, "hello world #%ld", value);
//        Serial.print("Publish message: ");
//        Serial.println(msg);
//        client.publish("outTopic", msg);
//      }
  

  
}

void lightIndicator()
{
  
    
    digitalWrite(indicatingLed,1);
    delay(200);
    digitalWrite(indicatingLed,0);
    delay(200);
    digitalWrite(indicatingLed,1);
    delay(200);
    digitalWrite(indicatingLed,0);
    delay(200);
    digitalWrite(indicatingLed,1);
    delay(200);
    digitalWrite(indicatingLed,0);
    delay(200);
    digitalWrite(indicatingLed,1);
    delay(200);
    digitalWrite(indicatingLed,0);
    delay(200);
    digitalWrite(indicatingLed,1);
    delay(200);
    digitalWrite(indicatingLed,0);
    delay(200);
    digitalWrite(indicatingLed,1);
    delay(200);
    digitalWrite(indicatingLed,0);
    delay(200);
    digitalWrite(indicatingLed,1);
    delay(200);
    digitalWrite(indicatingLed,0);  
}
