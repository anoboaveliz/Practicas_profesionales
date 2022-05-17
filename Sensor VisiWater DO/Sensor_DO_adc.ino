/***********************************************************
 DFRobot Gravity: Analog Current to Voltage Converter(For 4~20mA Application)
 SKU:SEN0262

 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/
#include <OneWire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;
const float multiplier = 0.1875F;

const int CurrentSensorPin = 36;
#define VREF 3300 // ADC's reference voltage on your Arduino,typical value:5000mV

unsigned int voltage; //unit:mV
float oxigeno; //unit:mg/l ppm
float corriente; //unit:mA

void setup()
{
   Serial.begin(115200);
   ads.setGain(GAIN_TWOTHIRDS);
   ads.begin();
}

void loop()
{
    int16_t adc0;
    adc0 = ads.readADC_SingleEnded(0);
      
    voltage = adc0*multiplier;
    Serial.print("voltage:");
    Serial.print(voltage);
    Serial.print(" mV  ");
    corriente= (voltage/1000.0)*(25.0/3.0); 
    Serial.print("current:");
    Serial.print(corriente);
    Serial.print(" mA  ");
    oxigeno = (((corriente)*(15.0/8.0))-(15.0/2.0));
    
    Serial.print("DO:");
    Serial.print(oxigeno);
    Serial.println(" mg/l ppm");
    delay(1000);
}
