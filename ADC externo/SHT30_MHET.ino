// VREF: Analog reference
// For Arduino UNO, Leonardo and mega2560, etc. change VREF to 5
// For Arduino Zero, Due, MKR Family, ESP32, etc. 3V3 controllers, change VREF to 3.3
#define VREF 3.3
#define TEMPERATURE_PIN 34
#define HUMIDITY_PIN 35
#define ADC_RESOLUTION 4096
#define RELAY_PIN 22

float Tc, Tf, RH, analogVolt;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando...");
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {

  analogVolt = (float)analogRead(TEMPERATURE_PIN) / ADC_RESOLUTION * VREF;
  // Convert voltage to temperature (℃, centigrade)
  Tc = -66.875 + 72.917 * (analogVolt+0.1);
  // Convert voltage to temperature (°F, fahrenheit )
  Tf = -88.375 + 131.25 * (analogVolt+0.1);
  Serial.print("Termperature:" );
  Serial.print(Tc, 1);
  Serial.print(" C / " );
  Serial.print(" V:");
  Serial.print(analogVolt);
  Serial.print("  ");
  Serial.print((float)analogRead(TEMPERATURE_PIN));
  
  /*Serial.print(Tf, 1);
  Serial.println(" F" );

  analogVolt = (float)analogRead(HUMIDITY_PIN) / ADC_RESOLUTION * VREF;
  // Convert voltage to relative humidity (%)
  RH = -12.5 + 41.667 * analogVolt;

  Serial.print("Humidity:" );
  Serial.print(RH, 1);
  Serial.println(" %RH" );*/

  Serial.println();
  delay(2000);
  /*digitalWrite(RELAY_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(5000); // wait for a second
  digitalWrite(RELAY_PIN, LOW); // turn the LED off by making the voltage LOW
  delay(5000); // wait for a secon*/
}
