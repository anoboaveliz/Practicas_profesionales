#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <WiFi.h>

Adafruit_ADS1115 ads;

float Voltage = 0.0;
const char* ssid = "Raspberry_ap";
const char* password =  "Prueba123";
const uint16_t port = 8090;
const char * host = "192.168.11.2";

void setup() {
  Serial.begin(115200);
  ads.setGain(GAIN_TWOTHIRDS);
  ads.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;
  if (!client.connect(host, port)) {
      Serial.println("Error de conexion con host");
      delay(1000);
      return;
  }
  
  int16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);
  Voltage = ((adc0 * 0.0001875F)/0.18)*7.8;
  
  //Serial.println("Enviando mensaje...");
  client.print(String(Voltage));
  //Serial.print("V: "); Serial.println(Voltage);
  //Serial.println("Finalizando conexión");
  client.stop();
}
