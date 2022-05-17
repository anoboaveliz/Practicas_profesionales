#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//SHT30
#define VREF 3.3
#define TEMPERATURE_PIN 34
#define HUMIDITY_PIN 35
#define ADC_RESOLUTION 4096
#define RELAY_PIN 23
float Tc, RH, analogVolt;

//MQTT
#define WIFISSID  //WIFI
#define PASS  //WIFI
#define MQTT_USER "TERMOHIDROMETRO"//Nombre del dispositivo
#define MQTT_PASSW //Token
#define MQTT_CLIENT_NAME "" //Vacio
#define MQTT_TOPIC "/api/devices"
#define VAR_LABEL_1 "Temperatura" //Nombre de variable 1
#define VAR_LABEL_2 "Humedad" //Nombre de variable 2
#define DEV_LABEL "TERMOHIDROMETRO" //Device Name = Device Label
char mqttBroker[];
char payload[100];
char topic[150];
char str_sensor1[10];
char str_sensor2[10];
WiFiClient sens;
PubSubClient client(sens);

//Mllis()
unsigned long previo = 0;
void callback(char* topic, byte* payload, unsigned int length){
  //Funcion que recibe los mensajes del brooker
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  //Serial.write(payload, length);
  //Serial.print(topic);

  Serial.println(message);
  if (message == "0"){
    digitalWrite(RELAY_PIN, HIGH);
  }
  else if(message == "1"){
    digitalWrite(RELAY_PIN, LOW);
  }
}

void reconnect(){
  while (!client.connected()){
    Serial.println("Intentando conexion MQTT...");

    if (client.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASSW)) {
      Serial.println("Connected");
      client.subscribe("/api/devices/TERMOHIDROMETRO/ventilador/value"); //Suscribirse a un topic
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando...");
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
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
  unsigned long actual = millis();
  client.loop(); //Loop que recibe mensajes


  while (actual-previo > 10000){
    previo=actual;
    sprintf(topic, "%s/%s", MQTT_TOPIC, DEV_LABEL);
    sprintf(payload, "%s", "");
    
  //*********************************TEMPERATURA*******************************************
    analogVolt = (float)analogRead(TEMPERATURE_PIN) / ADC_RESOLUTION * VREF;
   
    // Convert voltage to temperature (℃, centigrade)
    Tc = -66.875 + 72.917 * (analogVolt+0.1);
  
    Serial.print("Termperature:" );
    Serial.print(Tc, 1);
    Serial.print(" C / " );
  
  //*********************************HUMEDAD***********************************************
    analogVolt = (float)analogRead(HUMIDITY_PIN) / ADC_RESOLUTION * VREF;
    
    // Convert voltage to relative humidity (%)
    RH = -12.5 + 41.667 * (analogVolt+0.1);
  
    Serial.print("Humidity:" );
    Serial.print(RH, 1);
    Serial.println(" %RH" );
  
  //*********************************PUBLICAR*********************************************
    sprintf(payload, "{\"%s\":", VAR_LABEL_1);
    dtostrf(Tc, 4, 2, str_sensor1);
    sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor1);
    Serial.println("Publicando Temp en SensNextGen");
    Serial.println(payload);
    client.publish(topic, payload);
    client.loop();
  
    sprintf(payload, "%s", "");
    sprintf(payload, "{\"%s\":", VAR_LABEL_2);
    dtostrf(RH, 4, 2, str_sensor2);
    sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor2);
    Serial.println("Publicando Hum en SensNextGen");
    Serial.println(payload);
    client.publish(topic, payload);
    client.loop();
  
    
    Serial.println();
  }
    //delay(10000);
}
