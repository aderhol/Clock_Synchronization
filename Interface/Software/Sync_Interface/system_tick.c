#include <stdint.h>
#include "driverlib/systick.h"
#include "system_tick.h"
#include "UART_controller.h"

extern uint32_t SYS_CLK_FREQ_ACTUAL;

void SysTickInit(void) {
    SysTickPeriodSet(SYS_CLK_FREQ_ACTUAL / SYSTEM_TICK_FREQ);
    SysTickEnable();
    SysTickIntEnable();
}

extern void PPSLEDsService(void);

void ISR_SysTick(void) {
    UARTHandler();
    PPSLEDsService();
}
