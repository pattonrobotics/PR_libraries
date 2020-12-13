/* YourDuino.com Example Software Sketch
   DHT11 Humidity and Temperature Sensor test
   Credits: Rob Tillaart
   http://arduino-direct.com/sunshop/index.php?l=product_detail&p=162
   terry@yourduino.com
   Modified by Brian Patton 6/29/2020
   Supported libary functions
  .read(int pin);// gather the raw data
  .humidityWhole;    // Holder for the whole number in humidity
  .humidityDecimal;  // Holder for the decimal number in humidity
  .temperatureWhole; // Holder for the whole number in temperature
  .tempDecimal;    // Holder for the decimal number in temperature
  .temperatureC;   // Variable to hold the combined whole and decimal value for temperature in Celcius
  .temperatureF;   // Variable to hold the combined whole and decimal value for temperature in Fahrenheit
  .temperatureK;   // Variable to hold the combined whole and decimal value for temperature in Kelvin
  .humidity;     // Variable to hold the combined whole and decimal value for humidity
*/

/* Connection: As I have it
    DHT11
    S   -->  pin10     This is user selectable and need to be declared below
    Vcc -->  5 volts
    GND -->  Gound

*/

/*-----( Import needed libraries )-----*/
#include <dht11.h>
/*-----( Declare objects )-----*/
dht11 DHT11;
/*-----( Declare Constants, Pin Numbers )-----*/
const int DHT11PIN = 10; // mine is connected to pin 10
/*-----( variables )-----*/
int dlay = 2000;
//**********************************************************
//        Setup
//**********************************************************
void setup()   {
  Serial.begin(9600);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}
//**********************************************************
//        Main Loop
//**********************************************************
void loop()  {
  getDHTdata();
}
//**********************************************************
//        Print Data Function
//**********************************************************
void getDHTdata() {
  int chk = DHT11.read(DHT11PIN);   // Get all 5 8 bit bytes All the data is here!
  if (chk == 0) {
    Serial.println("\n");         // Everything looks good
    Serial.println("Read sensor: OK ");
    printdata();            // display of the most useful data
    delay(dlay);
  }
  else {
    Serial.print("error");
  }
}
//**********************************************************
//        Print Data Function
//**********************************************************
void printdata() {
  float humid = DHT11.humidity;     // Variable to hold the combined whole and decimal value for humidity
  float tC = DHT11.temperatureC;    // Variable to hold the combined whole and decimal value for temperature in Celcius
  float tF = DHT11.temperatureF;    // Variable to hold the combined whole and decimal value for temperature in Fahrenheit

  Serial.println("Humidity: " + String(humid, 2) + "%");
  Serial.println("Temperature: " + String(tC, 2) + "C");
  Serial.println("Temperature: " + String(tF, 2) + "F");
}
