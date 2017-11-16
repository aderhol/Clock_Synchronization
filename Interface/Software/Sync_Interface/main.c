#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h" //System Control
#include "driverlib/uart.h" //UART
#include "driverlib/gpio.h" //GPIO
#include "uart_io.h"  //UART functionalities
#include "pwm.h"
#include "latency.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/interrupt.h"    //interrupt API
#include "command.h"
#include "system_tick.h"
#include "driverlib/fpu.h"
#include "driverlib/pwm.h"
#include "i2c_io.h"

//#define PULSE_ON //makes the pulse command available, disables pwm

#define SYS_CLK_FREQ 120000000
uint32_t SYS_CLK_FREQ_ACTUAL;

void init(void);

//static char stringBuilder[101];

int main(void)
{
    init();

    UARTPrint(UART0_BASE, "Configured!\r\n\r\n");

    while(1)
    {

    }
}

void pulse(void)
{
#ifdef PULSE_ON
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0xF);
    SysCtlDelay(250);
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
    UARTPrint(UART0_BASE, " Pulse sent!\r\n");
#else
    UARTPrint(UART0_BASE, " This functionality is not available!\r\n");
#endif
}

void init(void)
{
    //set the System Clock Frequency to 120 MHz
    SYS_CLK_FREQ_ACTUAL = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), SYS_CLK_FREQ);

    FPUEnable();

    UARTInit();
    i2cInit();
    //latencyInit();
    SysTickInit();

#ifdef PULSE_ON
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG));
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
#else
    //PWMInit();
#endif

    IntMasterEnable();  //enables unmasked interrupts
}

void InterruptConfigFaultISR(const uint8_t* message)
{
    UARTPrint(UART0_BASE, "\r\n\r\nHard Fault: ");
    UARTPrint(UART0_BASE, message);
    UARTPrint(UART0_BASE, "\r\n");
    while(1);
}
