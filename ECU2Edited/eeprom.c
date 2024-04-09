#include "eeprom.h"
#include "tm4c123gh6pm.h"

void customDelay() {
    // 6 cycles plus function call overhead
    for (volatile uint32 i = 0; i < 6; ++i) {
    }
}

void EEPROMInit() {
    // Step 1: Insert delay
    SYSCTL_RCGCEEPROM_R |= 0x01;

    customDelay();
    // Step 2: Poll the WORKING bit in the EEPROM Done Status (EEDONE) register
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);

    // Step 3: Read the PRETRY and ERETRY bits in the EEPROM Support Control and Status (EESUPP) register
    if ((EEPROM_EESUPP_R & (EEPROM_EESUPP_PRETRY | EEPROM_EESUPP_ERETRY)) != 0) {
        // Error handling - return an error or take appropriate action
        return;  // Modify this based on your error handling strategy
    }
    // Step 4: Reset the EEPROM module using the EEPROM Software Reset (SREEPROM) register
    SYSCTL_SREEPROM_R = SYSCTL_SREEPROM_R0;

    // Step 5: Insert delay
    customDelay();
    // Step 6: Poll the WORKING bit in the EEPROM Done Status (EEDONE) register
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);

    // Step 7: Read the PRETRY and ERETRY bits in the EESUPP register
    if ((EEPROM_EESUPP_R & (EEPROM_EESUPP_PRETRY | EEPROM_EESUPP_ERETRY)) != 0) {
        // Error handling - return an error or take appropriate action
        return;  // Modify this based on your error handling strategy
    }

    // EEPROM initialization is complete, and software may use the peripheral as normal
}

//void EEPROMIntEnable(uint32 ui32IntFlags) {
//    Set the specified interrupt flags in the EEINTEN register
//    EEPROM_EEINTEN_R |= ui32IntFlags;
//}

void EEPROMProgram(uint32 address, uint32 eedata)
{
    EEPROM_EEBLOCK_R = address >> 4; // EEBLOCK: Bits 15:4 of the EEPROM address
    EEPROM_EEOFFSET_R = address & 0xF; // EEOFFSET: Bits 3:0 of the EEPROM address
    EEPROM_EERDWR_R = eedata;
    // Wait for the write operation to complete
    while (EEPROM_EEDONE_R & (1 << 0));  // Check WORKING bit

    // Check for errors in the EEDONE register
    if (EEPROM_EEDONE_R & (1 << 1)) {  // Check ERR bit

    }
}

uint32 EEPROMRead(uint32 address) {
    // Set the EEPROM block and offset based on the given address
    EEPROM_EEBLOCK_R = address >> 4;         // Bits 15:4 of the EEPROM address
    EEPROM_EEOFFSET_R = address & 0xF;       // Bits 3:0 of the EEPROM address

    // Trigger a read operation by writing a dummy value to EERDWR register
    EEPROM_EERDWR_R = 0;

    // Wait for the read operation to complete
    while (EEPROM_EEDONE_R & (1 << 0));  // Check WORKING bit

    // Check for errors in the EEDONE register
    if (EEPROM_EEDONE_R & (1 << 1)) {  // Check ERR bit
        // Handle the error, for example, by setting an error flag or taking appropriate action
        // You may also choose to return an error code here
        return 1;
    }

    // Return the read data
    return EEPROM_EERDWR_R;
}

uint32 EEPROMSizeGet(void) {
    // Read the EEPROM size from the hardware register
    uint32 eesize = EEPROM_EESIZE_R;

    // Return the size of the EEPROM in bytes
    return (1U << (eesize + 10));
}


uint32 EEPROMBlockCountGet(void) {
#ifdef EEPROM_SIZE_LIMIT
    // If a size limit has been specified, fake the number of blocks to match
    return (EEPROM_SIZE_LIMIT / 48);
#else
    // Read the EEPROM size from the hardware register
    uint32 eesize = EEPROM_EESIZE_R;

    // Calculate the number of blocks based on the EEPROM size
    // The formula may vary based on the specific EEPROM size representation in your hardware
    // This is a generic example; replace it with the correct formula for your hardware
    uint32 blockSize = (1U << (eesize + 10));

    return (1U << eesize) / blockSize;
#endif
}


// Function to perform EEPROM debug mass erase
uint32 EEPROMMassErase(void) {
    // Step 1: Reset the EEPROM module
    SYSCTL_SREEPROM_R = SYSCTL_SREEPROM_R0;

    // Step 2: Wait until WORKING bit in the EEPROM Done Status (EEDONE) register is clear
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);

    // Step 3: Enable debug mass erase by setting ME bit in the EEPROM Debug Mass Erase (EEDBGME) register
    EEPROM_EEDBGME_R |= 0x1;  // Set ME bit

    // Check for errors in the EEDONE register
    if (EEPROM_EEDONE_R & (1 << 1)) {  // Check ERR bit
        // Handle the error, for example, by setting an error flag or taking appropriate action
        return 1;  // Return a non-zero value to indicate an error
    }

    // Mass erase successful
    return 0;
}



uint32 EEPROMStatusGet(void) {
    // Read the status from the EEDONE register
    uint32 status = EEPROM_EEDONE_R;

    return status;
}

// Function to program a word of data into EEPROM non-blocking
uint32 EEPROMProgramNonBlocking(uint32 eedata, uint32 ui32Address) {
    // Set the EEPROM block and offset based on the given address
    EEPROM_EEBLOCK_R = ui32Address >> 4;         // Bits 15:4 of the EEPROM address
    EEPROM_EEOFFSET_R = ui32Address & 0xF;       // Bits 3:0 of the EEPROM address

    // Program data into EEPROM
    EEPROM_EERDWR_R = eedata;

    // Check for the WORKING bit to determine if the operation is in progress
    if (EEPROM_EEDONE_R & (1 << 0)) {  // Check WORKING bit
        // EEPROM programming is in progress
        return 1;  // Indicate in progress
    }

    // EEPROM programming complete
    return 0;  // Indicate success
}


// Function to set a password for a specific EEPROM block
uint32 EEPROMBlockPasswordSet(uint32 ui32Block, uint32 *pui32Password, uint32 ui32Count) {
    // Check if the block number is within the valid range
    if (ui32Block > 15) {
        return 1;  // Return an error code indicating an invalid block number
    }

    // Check if the password count is within the valid range (1-4)
    if (ui32Count < 1 || ui32Count > 4) {
        return 2;  // Return an error code indicating an invalid password count
    }

    // Set the EEPROM block for which the password is to be set
    EEPROM_EEBLOCK_R = ui32Block;

    // Set the passwords for the specified block
    switch (ui32Count) {
        case 1:
            EEPROM_EEPASS0_R = pui32Password[0];
            break;
        case 2:
            EEPROM_EEPASS0_R = pui32Password[0];
            EEPROM_EEPASS1_R = pui32Password[1];
            break;
        case 3:
            EEPROM_EEPASS0_R = pui32Password[0];
            EEPROM_EEPASS1_R = pui32Password[1];
            EEPROM_EEPASS2_R = pui32Password[2];
            break;

    }

    return 0;  // Return 0 to indicate success
}


// Function to disable EEPROM interrupts
void EEPROMIntDisable(uint32 ui32IntFlags) {
    // Clear the specified interrupt flags in the EEINT register
    EEPROM_EEINT_R &= ~ui32IntFlags;
}

// Function to clear EEPROM interrupts
void EEPROMIntClear(uint32 ui32IntFlags) {
    // Clear the specified interrupt flags in the EEINT register
    EEPROM_EEINT_R &= ~ui32IntFlags;

}
