/*
* Serial_Processing.cpp
*
* Created: 3/12/2019 4:43:35 PM
* Author: Anthony
*/

#include <Arduino.h>
#include "Serial_Processing.h"
#include "HardwareTypes.h"
#include "Device_Configuration.h"
#include "Serial_Commands.h"
#include "globals.h"




// default constructor
Serial_Processing::Serial_Processing()
{
} //Serial_Processing


void Serial_Processing::Setup(void)
{
	//Serial.begin(SERIAL_BAUD);               //Set the hardware serial port speed
	//Serial.setTimeout(50);
}


int Serial_Processing::CommandsProcess(sCommand *ptrCmds)
{


	if (Serial.available() > 0)
	{
		computer_bytes_received = Serial.readBytesUntil(10, computerdata, MAX_CMD_LENGTH); //We read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received
		computerdata[computer_bytes_received] = 0; //We add a 0 to the spot in the array just after the last character we received.. This will stop us from transmitting incorrect data that may have been left in the buffer
		
	}
	else
	{
		return 0;
	}

	if (computerdata != 0) {             //If computer_bytes_received does not equal zero
		
		CommandParse(ptrCmds, computerdata);
		
		computer_bytes_received = 0;                  //Reset the var computer_bytes_received to equal 0
		computerdata[computer_bytes_received] = {0};
	}

	return 0;
}


unsigned int Serial_Processing::CommandParse(sCommand *ptrCmds, char str[MAX_CMD_LENGTH])
{
	
	char str2[MAX_CMD_LENGTH] = {0};
	strcpy(str2, str);
	
	hardwareType = strtoke(str, DELIMITER); //hardware ID
	cmd = strtoke(NULL, DELIMITER);
	arguments = strtoke(NULL, DELIMITER);
	checksum = strtoke(NULL, DELIMITER);

	if (!checksumPassed(checksum, str2))
	return 0;

	for (int i=0; hardwareType[i]!= '\0'; i++)
	{
		//Serial.println(hardwareType[i]);
		if (!isdigit(hardwareType[i]) != 0)
		{
			Serial.println("Invalid Hardware ID, number is not a digit");
			return 0;
		}
	}

	if (MYHARDWARETYPE != atoi(hardwareType))
	{
		Serial.println("Invalid Hardware ID");
		return 0;
	}

	unsigned int i;
	sCommand cmd_list;
	i=0;

	memcpy(&cmd_list, &ptrCmds[i], sizeof(sCommand));
	
	bool argc = arguments[0] != '\0';
	while(cmd_list.function!=0)
	{
		if (strcicmp(cmd,cmd_list.name)==0){
			return (*cmd_list.function)(argc ? 1 : 0,cmd,arguments);
		}

		i=i+1;
		memcpy(&cmd_list, &ptrCmds[i], sizeof(sCommand));
	}
	

	return 1;
}

int Serial_Processing::strcicmp(char const *a, char const *b)
{
	for (;; a++, b++) {
		int d = tolower(*a) - tolower(*b);
		if (d != 0 || !*a)
		return d;
	}
}

char* strtoke(char *str, const char *delim)
{
	static char *start = NULL; /* stores string str for consecutive calls */
	char *token = NULL; /* found token */
	/* assign new start in case */
	if (str) start = str;
	/* check whether text to parse left */
	if (!start) return NULL;
	/* remember current start as found token */
	token = start;
	/* find next occurrence of delim */
	start = strpbrk(start, delim);
	/* replace delim with terminator and move start to follower */
	if (start) *start++ = '\0';
	/* done */
	return token;
}

bool checksumPassed (char *serialChecksum, char *serialStringToCheck)
{

	char str2[MAX_CMD_LENGTH] = {0};
	char str3[MAX_CMD_LENGTH] = {0};
	strcpy(str2, serialStringToCheck);


	if (serialChecksum == NULL || serialChecksum == "" || serialChecksum[0] == '\0')
	return false;

	int32_t tCount = 0;
	int32_t tokenPosition = 0;
	byte checksumValue = 0;
	byte strPtr = 0;
	for (int i = 0; i < MAX_CMD_LENGTH; ++i)
	{
		if (tCount != 3)
		{
			str3[strPtr++] = str2[i];
		}
		else
		{
			if (str2[i] == ';')
			{
				tCount++;
			}
		}
		if (str2[i] == ';' && tCount < 3)
		{
			tokenPosition = i;
			tCount++;
		}
	}
	for (int i = 0; i < MAX_CMD_LENGTH; ++i)
	{
		checksumValue = checksumValue ^ str3[i];
	}
	if (checksumValue != atoi(serialChecksum))
	return false;

	return true;

}


// default destructor
Serial_Processing::~Serial_Processing()
{
} //~Serial_Processing
