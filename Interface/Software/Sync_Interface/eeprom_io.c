#include <stdbool.h>
#include <stdint.h>
#include "driverlib/eeprom.h"
#include "driverlib/crc.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"

#include "eeprom_io.h"

void EEPROMIOInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CCM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0));

    //checks if EEPROM is healthy
    if(EEPROMInit() == EEPROM_INIT_ERROR)
        while(1);
}


void writeWithVerification(uint32_t address, uint16_t data)
{
    uint32_t bytes[2] = {(uint32_t)((uint8_t)data), (uint32_t)((uint8_t)(data >> 8))};
    uint32_t dataOut;

    if(address % 4)
        while(1);

    CRCConfigSet(CCM0_BASE, (CRC_CFG_INIT_0 | CRC_CFG_SIZE_8BIT | CRC_CFG_TYPE_P1021));
    dataOut = (CRCDataProcess(CCM0_BASE, bytes, 2, false) << 16) | data; //[CRC-data]
    if(EEPROMProgram(&dataOut, address, 4))
        while(1);
}

bool getVerifiedValue(uint32_t address, uint16_t *data_out)
{
    uint32_t data;
    if(address % 4)
        while(1);
    EEPROMRead(&data, address, 4);
    *data_out = (uint16_t) data;
    uint32_t bytes[2] = {(uint32_t)((uint8_t)data), (uint32_t)((uint8_t)(data >> 8))};
    CRCConfigSet(CCM0_BASE, (CRC_CFG_INIT_0 | CRC_CFG_SIZE_8BIT | CRC_CFG_TYPE_P1021));
    return (CRCDataProcess(CCM0_BASE, bytes, 2, false) == (data >> 16));

}
