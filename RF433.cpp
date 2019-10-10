/*
433RF - Libray including code for the transmitter and reciever
Created November 2016 by Frank F

*/

#include "Arduino.h"
#include "RF433.h"

Transmitter::Transmitter(int transPin , int ledPin )
{
	
	pinMode(transPin, OUTPUT);
	_pwmPin = transPin;
	_ledPin = ledPin;
	_p = 500;
}

 void Transmitter::sendInteger(int val)
{
	 if (val < 0)
		 val = 0;
	
	 Serial.println(val);

	String binVal = String(val, BIN);

	sendPreamble();
	delay(1000);

	for (int x = 1; x < binVal.length() + 1; x++)
	{

		if (binVal.substring(x - 1, x).equals("0"))
		{
			delay(_p);
			sendShortPulse();
		}

		else if (binVal.substring(x - 1, x).equals("1"))
		{
			delay(_p);
			sendLongPulse();
		}

		else
		{
			Serial.println("Exception Value is not in binary form");
			Serial.println(binVal);

		}
			

	}
	Serial.println(binVal);
	delay(1500);
	sendEndamble();
	delay(5000);
}

 void Transmitter::sendPreamble()
{
	for (int x = 0; x<4; x++)  // original 50
	{
		digitalWrite(_pwmPin, HIGH);
		digitalWrite(_ledPin, HIGH);
		delay(20);
		digitalWrite(_pwmPin, LOW);
		digitalWrite(_ledPin, LOW);
		delay(20);


	}
}
 void Transmitter::sendEndamble()
{
	 Serial.println("Sending Endamble");

	for (int x = 0; x<4; x++)
	{
		digitalWrite(_pwmPin, HIGH);
		digitalWrite(_ledPin, HIGH);
		delay(35);
		digitalWrite(_pwmPin, LOW);
		digitalWrite(_ledPin, LOW);
		delay(35);
	}


}
 void Transmitter::sendShortPulse()
{
	for (int x = 0; x<1; x++) // org 10
	{
		digitalWrite(_pwmPin, HIGH);
		digitalWrite(_ledPin, HIGH);
		Serial.println("High");
		delay(60); // Short Pulse
		digitalWrite(_pwmPin, LOW);
		digitalWrite(_ledPin, LOW);
		Serial.println("LOW");
		delay(20);
	}
}

void Transmitter::sendLongPulse()
{
	for (int x = 0; x<1; x++) // org 10
	{
		digitalWrite(_pwmPin, HIGH);
		digitalWrite(_ledPin, HIGH);
		Serial.println("High");
		delay(250); //Long Pulse
		digitalWrite(_pwmPin, LOW);
		digitalWrite(_ledPin, LOW);
		Serial.println("LOW");
		delay(100);
	}
}

///////////////////////////////////////////////////////////////////////////Reciever Class/////////////////////////////////////////////////////////////////////////////////////////////////

Reciever::Reciever(int analogPin, int ledPin)
{
	pinMode(ledPin, OUTPUT);
	_ledPin = ledPin;
	_analogPin = analogPin;
	
}


boolean Reciever::detectPreamble(long var)
{
	long dTime = var;

	if (dTime > 15 && dTime < 24)
		return true;


	return false;


}


boolean Reciever::detectEndamble(long var)
{

	long dTime = var;

	if (dTime < 36 && dTime > 32)
		return true;

	return false;


}

int Reciever::pulseToBinaryInt(long var)
{
	long dTime = var;
	//Serial.println(dTime);
	if (dTime > 75 && dTime < 300)
		return 1;

	else if (dTime < 70 && dTime > 40)
		return 0;



	return -5;
}

void Reciever::sendBinToArray(int bVal)
{

	if (bVal == 1 || bVal == 0)
	{
		 _binaryArray[_sToValCount] = bVal;
		_sToValCount++;
	}
}


int Reciever::convertBinArrayToInt(int temp[])
{
	int startVal = 64;
	int val = 0;


	for (int x = 0; x<7; x++)
	{
		if (temp[x] == 1)
		{
			val += startVal;
			//val += pow(2,sizee-1);
		}
		//Serial.print(temp[x]);
		startVal = startVal / 2;
		//sizee--;

	}


	return val;



}


int Reciever::getInt()
{
	long var = getPulseLength();
	boolean isDetected = false;
	Serial.println("Not Detected");
	while (!isDetected)
	{
		Serial.println("IsDetected");
		var = getPulseLength();

		int temp = pulseToBinaryInt(var);
		
		if (temp != 5)
			sendBinToArray(temp);

		//sendBinToString(temp);
		if (detectEndamble(var))
			isDetected = true;

	}

	int integerValue = convertBinArrayToInt(_binaryArray);

	//Serial.println(binaryString);



	_sToValCount = 0;

	memset(_binaryArray, 0, 7);

	return integerValue;

}



long Reciever::getPulseLength()
{
	_data = analogRead(_analogPin);
	long fTime = millis();
	while (_data>400)
	{
		_data = analogRead(_analogPin);
	}
	long lTime = millis();

	return lTime - fTime;


}

int Reciever::getValue()
{
	int isDone = 0;
	 
	while (isDone == 0)
	{
		Serial.println("Running");
		if (detectPreamble(getPulseLength()))
		{
			Serial.println("Started");

			_val = getInt();
			isDone == 1;
			//Serial.println(val);
			//Serial.println(binaryString);
			//binaryString.remove(0);

			return _val;

		}

	}

}



