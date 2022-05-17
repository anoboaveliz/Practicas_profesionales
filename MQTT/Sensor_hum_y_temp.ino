// MQTT
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define WIFISSID 
#define PASS 
#define MQTT_USER "TERMOHIDROMETRO"//Device Name = Device Label ----- mayusculas
#define MQTT_PASSW //Token
#define MQTT_CLIENT_NAME ""
#define MQTT_TOPIC "/api/devices"

#define VAR_LABEL_1 "Temperatura" //label device
#define VAR_LABEL_2 "Humedad"

#define DEV_LABEL 


char mqttBroker[];
char payload[100];
char topic[150];

char str_sensor1[10];
char str_sensor2[10];

WiFiClient sens;
PubSubClient client(sens);


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


/**************************************************************************/
/*
    @file       SHT30_TempRH_Read.ino
    @author     Henry (DFRobot)
    @version    V1.0
    @date       2019-08-12
    @copyright  Copyright (c) 2010 DFRobot Co.Ltd (https://www.dfrobot.com)
    @licence    The MIT License (MIT)
    @breif      This example read and print the temperature and relative humidity.

    This demo and related libraries are for DFRobot Gravity: Analog SHT30 Temperature & Humidity Sensor
    Check out the links below for tutorials and connection diagrams
    Product(CH): https://www.dfrobot.com.cn/
    Product(EN): https://www.dfrobot.com/

*/
/**************************************************************************/

// VREF: Analog reference
// For Arduino UNO, Leonardo and mega2560, etc. change VREF to 5
// For Arduino Zero, Due, MKR Family, ESP32, etc. 3V3 controllers, change VREF to 3.3
#define VREF 1.1
#define TEMPERATURE_PIN 32
#define HUMIDITY_PIN 33
#define ADC_RESOLUTION 4096        

float Tc, Tf, RH, analogVolt;

void setup() {
  Serial.begin(115200);
  Serial.println("SHT30 Starts up.");

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

void loop() {
  if(!client.connected()) reconnect();

  analogVolt = (float)analogRead(TEMPERATURE_PIN) / ADC_RESOLUTION * VREF;
  // Convert voltage to temperature (℃, centigrade)
  Tc = -66.875 + 72.917 * analogVolt;
  // Convert voltage to temperature (°F, fahrenheit )
  Tf = -88.375 + 131.25 * analogVolt;

  analogVolt = (float)analogRead(HUMIDITY_PIN) / ADC_RESOLUTION * VREF;
  // Convert voltage to relative humidity (%)
  RH = -12.5 + 41.667 * analogVolt;
  
  sprintf(topic, "%s/%s", MQTT_TOPIC, DEV_LABEL);
  sprintf(payload, "%s", "");

  Serial.print("Termperature:" );
  Serial.print(Tc, 1);
  Serial.print(" C / " );
  Serial.print(Tf, 1);
  Serial.println(" F" );
  Serial.print("Humidity:" );
  Serial.print(RH, 1);
  Serial.println(" %RH" );

  sprintf(payload, "{\"%s\":", VAR_LABEL_1);
  dtostrf(Tc, 4, 2, str_sensor1);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor2);

  Serial.println("Publicando data en SensNextGen");
  Serial.println(payload);
  client.publish(topic, payload);
  client.loop();

  sprintf(payload, "{\"%s\":", VAR_LABEL_2);
  dtostrf(Tc, 4, 2, str_sensor2);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor2);

  Serial.println("Publicando data en SensNextGen");
  Serial.println(payload);
  client.publish(topic, payload);
  client.loop();

  Serial.println();
  delay(2000);
}
