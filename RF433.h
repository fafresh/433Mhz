/*
 433RF - Libray including code for the transmitter and reciever
 Created November 2016 by Frank F

*/
#ifndef RF433_h
#define RF433_h

#include "Arduino.h"

class Transmitter
{
public:
	Transmitter(int transPin, int ledPin);
	void sendInteger(int val);
	
private:
	int _pwmPin;
	int _ledPin;
	int _p;
	void sendPreamble();
	void sendEndamble();
	void sendShortPulse();
	void sendLongPulse();
	


};

class Reciever
{
public:
	Reciever(int analogPin, int ledPin);
	int getValue();
	long getPulseLength();
	
private:
	int _analogPin;
	int _ledPin;
	int _val;
	int _sToValCount;
	unsigned int _data;
	int _binaryArray[7];
	boolean detectPreamble(long var);
	boolean detectEndamble(long var);
	int pulseToBinaryInt(long var);
	void sendBinToArray(int bVal);
	int convertBinArrayToInt(int temp[]);
	int getInt();
	//long getPulseLength();


	

};



#endif // !433RF_h

