#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#define WIFISSID 
#define PASS 
#define MQTT_USER ""//Device Name = Device Label ----- mayusculas
#define MQTT_PASSW //Token
#define MQTT_CLIENT_NAME ""
#define MQTT_TOPIC "/api/devices"

#define VAR_LABEL "sensor" //label device


#define DEV_LABEL "" 

Adafruit_ADS1115 ads;

char mqttBroker[];
char payload[100];
char topic[150];

char str_sensor[10];

WiFiClient sens;
PubSubClient client(sens);

const int sensor = 34;
const float multiplier = 0.1875F;
float current;
float DO;

void callback(char* topic, byte* payload, unsigned int length){
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.print(topic);
  
}

void reconnect(){
  while (!client.connected()){
    Serial.println("Intentando conexion MQTT...");

    if (client.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASSW)) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
  
}

void setup(){
  
  Serial.begin(115200);
  delay(200);
  ads.begin();
  
  WiFi.begin(WIFISSID, PASS);

  Serial.println();
  Serial.print("Conectando a Wifi...");

  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(500);
    };

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

}

void loop(){
  if(!client.connected()) reconnect();
  
  sprintf(topic, "%s/%s", MQTT_TOPIC, DEV_LABEL);
  sprintf(payload, "%s", "");

  short adc0 = ads.readADC_SingleEnded(0) * multiplier;
  rent = (adc0/120.0);
  DO = (current - 4)/0.2
  55;
  Serial.print("DO: ");
  Serial.print(DO);
  Serial.println(" mg/l");

  sprintf(payload, "{\"%s\":", VAR_LABEL);
  dtostrf(DO, 4, 2, str_sensor);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor);

  Serial.println("Publicando data en SensNextGen");
  Serial.println(payload);
  client.publish(topic, payload);
  client.loop();
  delay(1000);
}
