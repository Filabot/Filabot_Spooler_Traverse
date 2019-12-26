/*
 * EEPROM_Storage.cpp
 *
 * Created: 12/26/2019 1:48:55 PM
 *  Author: Anthony
 */ 

 #include "EEPROM_Storage.h"
 #include <Arduino.h>
 #include "EEPROM.h"
 #include "Globals.h"


 EEPROM_Storage *EEPROM_Storage::firstInstance;

 

 EEPROM_Storage::EEPROM_Storage()
 {
	 if(!firstInstance)
	 {
		 firstInstance = this;
	 }
 }

 void EEPROM_Storage::init(){
	 
	 if (EEPROM.read(0) == 255) // flash bytes will be 255 at first run
	 {
		 __EEPROM_Storage.Inner_Traverse_Offset = INNER_TRAVERSE_OFFSET;
		 __EEPROM_Storage.Spool_Width = SPOOL_WIDTH;
		 __EEPROM_Storage.Start_Position = START_POSITION;
		
		 EEPROM.put(4, __EEPROM_Storage);
		 EEPROM.put(0, 0);
		 
		 
	 }

	 ReadStorage();
 }

 bool EEPROM_Storage::SaveStorage(void)
 {
	 EEPROM.put(4, __EEPROM_Storage);
	 ReadStorage();
	 return true;
 }


 bool EEPROM_Storage::ReadStorage(void)
 {
	 
	 EEPROM.get(4, __EEPROM_Storage); // byte array which is read from flash at address 4
	 START_POSITION = __EEPROM_Storage.Start_Position;
	 SPOOL_WIDTH = __EEPROM_Storage.Spool_Width;
	 INNER_TRAVERSE_OFFSET = __EEPROM_Storage.Inner_Traverse_Offset;
	 

	 return true;
 }
 // default destructor
 EEPROM_Storage::~EEPROM_Storage()
 {
 } //~EEPROM_Storage