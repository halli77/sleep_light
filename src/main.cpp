#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"


const char *wifi_network_name = WIFI_NETWORK_NAME;
const char *wifi_password = WIFI_PASSWORD;

const char *mqtt_server = MQTT_SERVER;
const int  mqtt_port = MQTT_PORT;
const char *mqtt_client_name = MQTT_CLIENT_NAME;
const char *mqtt_user = MQTT_USER;
const char *mqtt_password = MQTT_PASSWORD;
const char *mqtt_topic_color =  MQTT_TOPIC_COLOR;



boolean messageReceived = false;
String currentColor = "";


WiFiClient client;
PubSubClient mqttclient(client);


void onMessageReceived(char* topic, byte* payload, unsigned int length) {
    String message;

    Serial.print("Message received [");
    Serial.print(topic);
    Serial.print("] ");
    
    for (int i=0;i<length;i++) {         
       message += (char)payload[i];
    } 

    Serial.println(message);
    Serial.println();
    
    currentColor = message;
    messageReceived = true;
   
}


void switchLED(String color) {
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);

    if(color == "RED") {
        digitalWrite(D5, HIGH);
    }

    if(color == "YELLOW") {
        digitalWrite(D6, HIGH);
    }

    if(color == "GREEN") {
        digitalWrite(D7, HIGH);
    }

}



void setup() {
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    Serial.println("Starting up sleep_traffic_light");

    /* connect to Wifi */
    Serial.print("Connecting to ");
    Serial.println(wifi_network_name);
    WiFi.begin(wifi_network_name, wifi_password);

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    /* connect to mqtt */
    mqttclient.setServer(mqtt_server, mqtt_port);
    mqttclient.setCallback(onMessageReceived);
    while (!client.connected()) {
        Serial.print("Connect to MQTT on ");
        Serial.println(mqtt_server);
        if (mqttclient.connect(mqtt_client_name, mqtt_user, mqtt_password )) {
            Serial.println("connected!");
            Serial.print("Subscribe for ");
            Serial.println(MQTT_TOPIC_COLOR);
            mqttclient.subscribe(MQTT_TOPIC_COLOR);
            
        } else {
            Serial.print("Could not connect to MQTT. Status: ");
            Serial.println(mqttclient.state());
        }
    }


}







void loop() {
    
    mqttclient.loop();

    if(messageReceived) {
        messageReceived = false;
        switchLED(currentColor);
    }

    if(currentColor=="") {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
    } else {
        delay(5000);
    }

    
  
}






 
 