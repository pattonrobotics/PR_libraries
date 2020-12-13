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
 *  DHT11 
 *  S   -->  pin2     This is user selectable and need to be declared below
 *  Vcc -->  5 volts
 *  GND -->  Gound
 *  
 */

/*-----( Import needed libraries )-----*/
#include <dht11.h>
/*-----( Declare objects )-----*/
dht11 DHT11;
/*-----( Declare Constants, Pin Numbers )-----*/
const int DHT11PIN = 2; // mine is connected to pin 2
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
  int chk = DHT11.read(DHT11PIN);   // Get all 5 8 bit bytes All the data is here!
  switch (chk)
  {
    case 0:
      Serial.println("\n");         // Everything looks good
      Serial.println("Read sensor: OK ");
      printdata();            // display of the most useful data
      printRaw();             // display of the raw data
      delay(dlay);
      break;
    case -1:                        // The last byte is a checksum
      Serial.print(".");
      //      Serial.println("Checksum error");
      delay(dlay);
      break;
    case -2:                        // nothing came back when called
      Serial.print(".");
      //      Serial.println("Time out error");
      delay(dlay);
      break;
    default:                        // no idea what is wrong
      Serial.print(".");
      //      Serial.println("Unknown error");
      delay(dlay);
      break;
  }
}
//**********************************************************
//        Print Data Function
//**********************************************************
void printdata() {
  float humid = DHT11.humidity;     // Variable to hold the combined whole and decimal value for humidity
  float tC = DHT11.temperatureC;    // Variable to hold the combined whole and decimal value for temperature in Celcius
  float tF = DHT11.temperatureF;    // Variable to hold the combined whole and decimal value for temperature in Fahrenheit
  float tK = DHT11.temperatureK;    // Variable to hold the combined whole and decimal value for temperature in Kelvin
  float dP = dewPoint(tC, humid); // float variable to hold the calculated dew point
  float fDP = dewPointFast(tC, humid); // float variable to hold the calculated fast dew point

  Serial.println("Humidity: " + String(humid, 2) + "%");
  Serial.println("Temperature: " + String(tC, 2) + "C");
  Serial.println("Temperature: " + String(tF, 2) + "F");
  Serial.println("Temperature: " + String(tK, 2) + "K");
  Serial.println("Dew Point: " + String(dP, 2) + "C");
  Serial.println("Dew PointFast: " + String(fDP, 2) + "C");

}
//**********************************************************
// Raw Data Outputs
//**********************************************************
void printRaw() {
  int tCW = DHT11.temperatureWhole;    // float variable to hold the raw parsed temperature in celcius data
  int tCD = DHT11.tempDecimal;    // float variable to hold the raw parsed temperature decimal value
  int hW = DHT11.humidityWhole;    // float variable to hold the raw parsed temperature in celcius data
  int hD = DHT11.humidityDecimal;    // float variable to hold the raw parsed temperature decimal value
  Serial.print("Binary raw data for the temperature whole number : ");
  Serial.println(tCW, BIN);
  Serial.print("Binary raw data for the temperature Decimal number : ");
  Serial.println(tCD, BIN);
  Serial.print("Binary raw data for the humidity whole number : ");
  Serial.println(hW, BIN);
  Serial.print("Binary raw data for the humidity Decimal number : ");
  Serial.println(hD, BIN);
}
//**********************************************************
// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm
//**********************************************************
float dewPoint(float celsius, float humidity)
{
  float A0 = 373.15 / (273.15 + celsius);
  float SUM = -7.90298 * (A0 - 1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1) ;
  SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1) ;
  SUM += log10(1013.246);
  float VP = pow(10, SUM - 3) * humidity;
  float T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}
//**********************************************************
// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
//**********************************************************
float dewPointFast(float celsius, float humidity)
{
  float a = 17.271;
  float b = 237.7;
  float temp = (a * celsius) / (b + celsius) + log(humidity / 100);
  float Td = (b * temp) / (a - temp);
  return Td;
}

/* ( THE END ) */
