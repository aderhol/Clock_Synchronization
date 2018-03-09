#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h" //System Control
#include "driverlib/uart.h" //UART
#include "driverlib/gpio.h" //GPIO
#include "driverlib/watchdog.h"
#include "uart_io.h"  //UART functionalities
#include "latency.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/interrupt.h"    //interrupt API
#include "command.h"
#include "system_tick.h"
#include "driverlib/fpu.h"
#include "driverlib/pwm.h"
#include "i2c_io.h"
#include "pps_leds.h"
#include "clock.h"
#include "eeprom_io.h"
#include "command.h"

//#define WDT_OFF

#define SYS_CLK_FREQ 120000000
uint32_t SYS_CLK_FREQ_ACTUAL;

void init(void);

int main(void)
{
    init();
    setUp(false);
    if(SysCtlResetCauseGet() & SYSCTL_CAUSE_WDOG0){
        SysCtlResetCauseClear(SYSCTL_CAUSE_WDOG0);
        UARTPrint(UART0_BASE, "\r\n\r\n########################## WDT RESET #########################\r\n\r\n");
    }
    UARTPrint(UART0_BASE, "Configured!\r\n\r\n");

    while(1)
    {
#ifndef WDT_OFF
        WatchdogReloadSet(WATCHDOG0_BASE, SYS_CLK_FREQ_ACTUAL / 2);
#endif
    }
}

void wdtInit(void)
{
    //
    // Enable the Watchdog 0 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    //
    // Wait for the Watchdog 0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WDOG0));
    //
    // Check to see if the registers are locked, and if so, unlock them.
    //
    if(WatchdogLockState(WATCHDOG0_BASE) == true)
    {
        WatchdogUnlock(WATCHDOG0_BASE);
    }
    //
    // Initialize the watchdog timer.
    //
    WatchdogReloadSet(WATCHDOG0_BASE, SYS_CLK_FREQ_ACTUAL / 2);
    //
    // Enable the reset.
    //
    WatchdogResetEnable(WATCHDOG0_BASE);
    //
    // Enable the watchdog timer.
    //
    WatchdogEnable(WATCHDOG0_BASE);
}

void init(void)
{
    //set the System Clock Frequency to 120 MHz
    SYS_CLK_FREQ_ACTUAL = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), SYS_CLK_FREQ);

    FPUEnable();

#ifndef WDT_OFF
    wdtInit();
#endif

    EEPROMIOInit();
    UARTInit();
    i2cInit();
    latencyInit();
    PPSLEDsInit();
    clockInit();
    SysTickInit();

    IntMasterEnable();  //enables unmasked interrupts
}

void InterruptConfigFaultISR(const uint8_t* message)
{
    UARTPrint(UART0_BASE, "\r\n\r\nHard Fault: ");
    UARTPrint(UART0_BASE, message);
    UARTPrint(UART0_BASE, "\r\n");
    while(1);
}
