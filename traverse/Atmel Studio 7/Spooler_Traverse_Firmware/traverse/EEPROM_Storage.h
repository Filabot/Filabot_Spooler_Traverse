/*
 * EEPROM_Storage.h
 *
 * Created: 12/26/2019 1:46:02 PM
 *  Author: Anthony
 */ 


#ifndef EEPROM_STORAGE_H_
#define EEPROM_STORAGE_H_

#include <Arduino.h>
#include "Globals.h"

typedef struct
{
	uint32_t Inner_Traverse_Offset;
	uint32_t Spool_Width;
	startPosition_t Start_Position;

} _EEPROM_Storage;

class EEPROM_Storage {

	public:
	EEPROM_Storage();
	~EEPROM_Storage();
	bool SaveStorage(void);
	void init();
	_EEPROM_Storage __EEPROM_Storage;

	private:
	static EEPROM_Storage *firstInstance;
	EEPROM_Storage( const EEPROM_Storage &c );
	EEPROM_Storage& operator=( const EEPROM_Storage &c );
	bool ReadStorage(void);
	

};

#endif /* EEPROM_STORAGE_H_ */