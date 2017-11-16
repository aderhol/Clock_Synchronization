#include <stdint.h>
#include "driverlib/systick.h"

#define SYSTEM_TICK_FREQ 10 //[Hz] minimum 7. maximum SYS_CLK_FREQ_ACTUAL

extern uint32_t SYS_CLK_FREQ_ACTUAL;

void SysTickInit(void) {
    SysTickPeriodSet(SYS_CLK_FREQ_ACTUAL / SYSTEM_TICK_FREQ);
    SysTickEnable();
    SysTickIntEnable();
}

extern void CommandInterpreter(void);
extern void UARTTransferGPSData(void);

void ISR_SysTick(void) {
    CommandInterpreter();
    UARTTransferGPSData();
}
