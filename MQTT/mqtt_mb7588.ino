#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define WIFISSID 
#define PASS 
#define MQTT_USER "ULTRASONICO"//Device Name = Device Label ----- mayusculas
#define MQTT_PASSW //Token
#define MQTT_CLIENT_NAME ""
#define MQTT_TOPIC "/api/devices"

#define VAR_LABEL "mb7588" //label device


#define DEV_LABEL "ULTRASONICO" 


char mqttBroker[];
char payload[100];
char topic[150];

char str_sensor[10];

WiFiClient sens;
PubSubClient client(sens);

const int sensor = 32;
long sensor1, cm, inches;

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

long read_sensor(){
  sensor1 = pulseIn(sensor, HIGH);
  cm = sensor1/10; // converts the range to cm
  inches = cm/2.54; // converts the range to inches
  return cm;
}

void printall(){         
  Serial.print("S1");
  Serial.print(" ");
  Serial.print(sensor1); //This can be made to match the measurement type you wish to have.
  Serial.println("mm"); //Typically will be the range reading increments.
}

void setup(){
  
  Serial.begin(115200);
  pinMode(sensor, INPUT);
  delay(200);
 
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

  /* call the funtion readDistance() */
  cm = read_sensor();
  printall();
  
  sprintf(topic, "%s/%s", MQTT_TOPIC, DEV_LABEL);
  sprintf(payload, "%s", "");

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  sprintf(payload, "{\"%s\":", VAR_LABEL);
  dtostrf(cm, 4, 2, str_sensor);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor);

  Serial.print("Topic: "); Serial.println(topic);
  Serial.print("Payload: "); Serial.println(payload);
  Serial.println("Publicando data en SensNextGen");
  client.publish(topic, payload);
  client.loop();
  delay(10000);
}
