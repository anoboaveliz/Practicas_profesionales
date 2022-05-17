#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial BT; // Objeto Bluetooth
bool ver=0;

void setup() {
  Serial.begin(115200); // Inicialización de la conexión en serie para la depuración
  BT.begin("ESP32_BT"); // Nombre de su dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejarse");
}

void loop() {
  while (BT.available()) // Compruebe si recibimos algo de Bluetooth
  {
    char incoming = BT.read(); // Lee lo que recibimos
    Serial.print(incoming);
    ver = 1;
  }
  if (ver)
  {
    ver=0;  
  }
  delay(20);
}
