#ifndef EEPROM_IO
#define EEPROM_IO

#include <stdint.h>
#include <stdbool.h>

#define GPSBASE_EEPROM_ADDRESS      0
#define LOOPBACK_EEPROM_ADDRESS     4
#define SHOWGPS_EEPROM_ADDRESS      8
#define SHOWLINE_EEPROM_ADDRESS     12

void EEPROMIOInit(void);
bool getVerifiedValue(uint32_t, uint16_t*);
void writeWithVerification(uint32_t, uint16_t);

#endif
