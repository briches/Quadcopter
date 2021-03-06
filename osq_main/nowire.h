/*======================================================================
     NoWire library
     OpenSourceQuad
     -------------------------------------------------------------------*/
/*================================================================================
 
     Author		: Brandon Riches
     Date		: August 2013
     License		: GNU Public License
 
     This library is designed to allow easy communication between two
     microcontrollers through wireless serial protocol. It should abstract away the serial
     communication part of the interation, and leave just the message send/recieve
     functionality easily accessible.
 
     Copyright (C) 2013  Brandon Riches
 
     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
     
     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
     -----------------------------------------------------------------------------*/
#ifndef NOWIRE_H_INCLUDED
#define NOWIRE_H_INCLUDED

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*================================================================================
 	Serial3 settings
 	-----------------------------------------------------------------------------*/
#define BAUD		(57600)			// Baud rate of XB
#define	PAN_ID		(1234)			// Network ID for communication

/*================================================================================
 	Communication settings
 	-----------------------------------------------------------------------------*/
#define MSG_SIZE	(5)			// Number of bytes in each message
                                // | Start_Char | Message ID | Data * 3 |
#define START_CHAR	(0xFF)			// Signifies start of message
#define timeoutMicros   (500000)    // Number of microseconds to wait for more data once a message is started


/*================================================================================
 	Message content
 	-----------------------------------------------------------------------------*/
// Create your messages here.
enum messages  // Customize these.
{
	disarm = 0x00,
	autoland = 0x01,
	start = 0x02,
	broadcastData = 0x03,
	increaseOperatingPoint = 0x0C,
	decreaseOperatingPoint = 0x0D,
	increasePitch = 0x0E,
	decreasePitch = 0x0F,
	increaseRoll = 0x10,
	decreaseRoll = 0x11,

	activateAltitudeHold = 0x12,
        deactivateAltitudeHold = 0x13,

	increaseP = 0xA1,
	increaseI = 0xA2,
	increaseD = 0xA3,
	decreaseP = 0xA4,
	decreaseI = 0xA5,
	decreaseD = 0xA6,
        
	
	resetPitchRoll = 0xC0,
	err = -1, // Must have this one
};

enum {  FIRSTBYTE, M_ID, DATA1, DATA2, DATA3};

class NoWire
{
	public:
		NoWire();
		int ScanForMessages();
		bool start();

		unsigned char newMessage[MSG_SIZE];
		long timestamp;
};

NoWire :: NoWire() {
};

int NoWire :: ScanForMessages()
{
	if(Serial1.available() >= MSG_SIZE)
	{
		unsigned char firstByte = Serial1.read();
		if(firstByte == START_CHAR)
		{
			timestamp = micros();
			newMessage[FIRSTBYTE] = firstByte;
			newMessage[M_ID] = Serial1.read();
			newMessage[DATA1] = Serial1.read();
			newMessage[DATA2] = Serial1.read();
			newMessage[DATA3] = Serial1.read();
		}
		return newMessage[M_ID];
	}
	return err; // Full message not yet received;
};

bool NoWire :: start()
{
	Serial1.begin(BAUD);
	return 1;
};

NoWire receiver;

#endif // NOWIRE_H_INCLUDED

