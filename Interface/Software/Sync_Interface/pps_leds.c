#include <stdint.h>
#include <stdbool.h>
#include "pps_leds.h"
#include "system_tick.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/gpio.h" //GPIO
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h" //System Control

#define LED_ON_TIME 200 //[ms]
#define N (SYSTEM_TICK_FREQ * LED_ON_TIME / 1000)

static int8_t directState = 0, feedbackState = 0;

void PPSDirectkReceived(void)
{
    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, 0xFF);
    directState = N;
}

void PPSFeedbackReceived(void)
{
    GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, 0xFF);
    feedbackState = N;
}

void PPSLEDsService(void)
{
    if(directState)
            if(!(--directState))
                GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, 0);

    if(feedbackState)
        if(!(--feedbackState))
            GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, 0);
}

void PPSLEDsInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ));
    GPIOPinTypeGPIOOutput(GPIO_PORTP_BASE, GPIO_PIN_3);     //feedback
    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_1);     //direct
    GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, 0);
    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, 0);
}
