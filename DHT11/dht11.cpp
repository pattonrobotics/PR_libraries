//
//    FILE: dht11.cpp
// VERSION: 0.3.2
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
// HISTORY:
// George Hadjikyriacou - Original version (??)
// Mod by SimKard - Version 0.2 (24/11/2010)
// Mod by Rob Tillaart - Version 0.3 (28/03/2011)
// + added comments
// + removed all non DHT11 specific code
// + added references
// Mod by Brian Patton 6/29/2020
// + Added bit 3 to the sum for checksum error. Some devices have a decimal value for both temp and humidity.
// my device had a decimal value for temperature.

#include "dht11.h"

// returnvalues:
//  0 : OK
// -1 : checksum error
// -2 : timeout
int dht11::read(int pin)
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(pin) == LOW)
		if (loopCnt-- == 0) return -2;

	loopCnt = 10000;
	while(digitalRead(pin) == HIGH)
		if (loopCnt-- == 0) return -2;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(pin) == LOW)
			if (loopCnt-- == 0) return -2;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(pin) == HIGH)
			if (loopCnt-- == 0) return -2;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
        // as bits[1] and bits[3] are allways zero they are omitted in formulas.
		// Update: Some devices have decimal values in bits 1 and 3
		// Re-named by Brian Patton
	humidityWhole    = bits[0]; 
	humidityDecimal    = bits[1];
	temperatureWhole = bits[2];
	tempDecimal = bits[3];	


	//**********************************************************
	//Combine Whole numbers and decimal numbers for Temperature
	// added by Brian Patton
	//**********************************************************
  if(tempDecimal == 0){
    temperatureC = temperatureWhole + tempDecimal;
  }
  else if(tempDecimal < 10){
    temperatureC = temperatureWhole + tempDecimal/10.0;
  }
  else if(tempDecimal < 100){
    temperatureC = temperatureWhole + tempDecimal/100.0;
  }
  else{
    temperatureC = temperatureWhole + tempDecimal/1000.0;
  }
	//**********************************************************
	//Combine Whole numbers and decimal numbers for Humidity
	// added  by Brian Patton
	//**********************************************************
  if(humidityDecimal == 0){
    humidity = humidityWhole + humidityDecimal;
  }
  else if(humidityDecimal < 10){
    humidity = humidityWhole + humidityDecimal/10.0;
  }
  else if(humidityDecimal < 100){
    humidity = humidityWhole + humidityDecimal/100.0;
  }
  else{
    humidity = humidityWhole + humidityDecimal/1000.0;
  }	
	//**********************************************************
	//Celsius to Fahrenheit conversion
	// added by Brian Patton
	//**********************************************************
	temperatureF =1.8 * temperatureC  + 32;
	//**********************************************************
	//Celsius to Kelvin conversion
	// added by Brian Patton
	//**********************************************************
	temperatureK = temperatureC + 273.15;

	uint8_t sum = bits[0] + bits[1]  + bits[2] + bits[3];  // added bits 1 and 3 for a decimal value

	if (bits[4] != sum) return -1;
	return 0;
}
//
// END OF FILE
//