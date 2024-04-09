#ifndef EEPROM_H
#define EEPROM_H
#include "tm4c123gh6pm.h"
#include "Std_Types.h"

#define ERROR_LOCATION_EEPROM 0
#define ERROR_BLOCK_EEPROM 0

void eeprom_init(void);
void eeprom_write(int data,uint8 addr,uint8 blk);
int eeprom_read(uint8 addr,uint8 blk);

#endif
