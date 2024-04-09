#ifndef EEPROM_H_
#define EEPROM_H_
#include "Std_Types.h"
#include "tm4c123gh6pm.h"



void EEPROMInit(void);
void EEPROMProgram(uint32 address, uint32 eedata);
uint32 EEPROMRead(uint32 address);
uint32 EEPROMSizeGet(void);
uint32 EEPROMBlockCountGet(void);
uint32 EEPROMMassErase(void);
uint32 EEPROMStatusGet(void);
uint32 EEPROMProgramNonBlocking(uint32 eedata, uint32 ui32Address);
uint32 EEPROMBlockPasswordSet(uint32 ui32Block, uint32 *pui32Password, uint32 ui32Count);
void EEPROMIntDisable(uint32 ui32IntFlags);
void EEPROMIntClear(uint32 ui32IntFlags);



//void EEPROMIntEnable (uint32 ui32IntFlags)
//uint32_t EEPROMBlockProtectGet(uint32_t ui32Block);
//uint32_t EEPROMBlockProtectSet(uint32_t ui32Block,uint32_t ui32Protect);
//uint32_t EEPROMBlockLock(uint32_t ui32Block);
//uint32_t EEPROMBlockUnlock(uint32_t ui32Block,uint32_t *pui32Password,uint32_t ui32Count);
//void EEPROMBlockHide(uint32_t ui32Block);
//uint32_t EEPROMIntStatus(bool bMasked);

#endif
