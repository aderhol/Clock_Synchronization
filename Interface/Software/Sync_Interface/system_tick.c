#include <stdint.h>
#include "driverlib/systick.h"
#include "system_tick.h"

extern uint32_t SYS_CLK_FREQ_ACTUAL;

void SysTickInit(void) {
    SysTickPeriodSet(SYS_CLK_FREQ_ACTUAL / SYSTEM_TICK_FREQ);
    SysTickEnable();
    SysTickIntEnable();
}

extern void CommandInterpreter(void);
extern void UARTTransferGPSData(void);
extern void UARTTransferLineData(void);
extern void PPSLEDsService(void);

void ISR_SysTick(void) {
    CommandInterpreter();
    UARTTransferGPSData();
    UARTTransferLineData();
    PPSLEDsService();
}
