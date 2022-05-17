#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial BT; // Objeto Bluetooth
String clientName = "visionsom-8mm-cb";
bool connected;

void setup() {
  Serial.begin(115200); // Inicialización de la conexión en serie para la depuración
  BT.begin("ESP32_bt", true); // Nombre de su dispositivo Bluetooth y en modo maestro
  Serial.println("El dispositivo Bluetooth está en modo maestro. Conectando con el anfitrión ...");
  connected = BT.connect(clientName);
  if(connected) {
    Serial.println("¡Conectado exitosamente!");
  } else {
    while(!BT.connected(10000)) {
      Serial.println("No se pudo conectar. Asegúrese de que el dispositivo remoto esté disponible y dentro del alcance, luego reinicie la aplicación."); 
    }
  }
}

void loop() {
  delay(500);
  BT.write(49); // Envía 1 en ASCII
  delay(500);
  BT.write(48); // Envía 0 en ASCII
}
