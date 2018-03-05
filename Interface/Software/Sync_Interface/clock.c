#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "clock.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/gpio.h" //GPIO
#include "driverlib/timer.h"    //timer
#include "driverlib/sysctl.h" //System Control
#include "inc/hw_memmap.h"

static int sync = 0;

void clockInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);    //enables GPIOD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);   //enables TIMER 3

    //waits until peripheries are ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER3));

    //configures timers for two-signal edge-separation
    /*GPIOPinConfigure(GPIO_PD4_T3CCP0);   //sets PD4 as the input capture input for TIMER3
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_4); //configures the pins*/
    TimerClockSourceSet(TIMER3_BASE, /*TIMER_CLOCK_PIOSC*/TIMER_CLOCK_SYSTEM);  //TIMER3 is clocked from the system clock
    TimerConfigure(TIMER3_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM)); //configures TIMER3


    TimerLoadSet (TIMER3_BASE, TIMER_A, 0x9680);
    TimerPrescaleSet(TIMER3_BASE, TIMER_A, 0x98);
    TimerMatchSet(TIMER3_BASE, TIMER_A, 0x4B40);
    TimerPrescaleMatchSet(TIMER3_BASE, TIMER_A, 0x4C);


    TimerEnable(TIMER3_BASE, TIMER_A);
}

void clockDisconnect(void)
{
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_4);
}

void clockConnect(void)
{
    sync = 1;
    GPIOPinConfigure(GPIO_PD4_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_4); //configures the pins
}

void clockSync(void)
{
    if(sync){
        TimerSynchronize(TIMER0_BASE, TIMER_3A_SYNC);
        sync = 0;
    }
}
