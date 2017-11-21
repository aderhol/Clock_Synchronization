#include <stdint.h>
#include <stdbool.h>
#include "gps1.h"
#include "system_tick.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/gpio.h" //GPIO
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h" //System Control

#define RESET_TIME 3000 //[ms]
#define N (SYSTEM_TICK_FREQ * RESET_TIME / 1000 + 1)

static int32_t count;

void GPS1Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG));
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0xFF);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0xFF);
}

void GPS1Reset(void)
{
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
    count = N;
}

void GPS1Service(void)
{
    if(count)
        if(!(--count))
            GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0xFF);
}
