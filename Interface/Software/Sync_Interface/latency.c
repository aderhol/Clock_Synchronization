#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "latency.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"    //interrupt defines (eg. INT_UART0)
#include "driverlib/timer.h"    //timer
#include "driverlib/interrupt.h"    //interrupt API
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/gpio.h" //GPIO
#include "driverlib/sysctl.h" //System Control
#include "uart_io.h"

extern void InterruptConfigFaultISR(const uint8_t*);
extern uint32_t SYS_CLK_FREQ_ACTUAL;

static char stringBuilder[101];

#define INPUT_CAPTURE_TIMER_WIDTH 16777216 //24 bit wide -> 2^24 = 16,777,216
#define INPUT_CAPTURE_CLOCK_FREQUENCY SYS_CLK_FREQ_ACTUAL
static int32_t offset = 0, timestamp = -1;
static int8_t measuring = 0;

/*void measureLatency(int8_t c)
{
    measuring += c;
}*/

//overflow
void ISR_TIMER0_A(void)
{
    uint32_t callers = TimerIntStatus(TIMER0_BASE, true);   //determines what triggered the interrupt
    TimerIntClear(TIMER0_BASE, callers);    //clears the interrupt flags

    if(callers & TIMER_TIMA_TIMEOUT) { //timer timeout
        if(timestamp != -1) {  //edge on A captured
            if(offset) { //an overflow already occurred
                timestamp = -1;
                offset = 0;
                UARTPrint(UART0_BASE, "\r\n> ERROR: timeout\r\n\r\n");
            }
            else
                offset += INPUT_CAPTURE_TIMER_WIDTH;
        }
        else
            TimerIntClear(TIMER2_BASE, TIMER_CAPA_EVENT & TIMER_CAPB_EVENT);    //prevents the usage of a B event which occurred before the captured A and before the overflow
    }
    else {
        sprintf(stringBuilder, "TIMER0 unexpected interrupt: 0x%X", callers);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
    if(callers - TIMER_TIMA_TIMEOUT) {  //unexpected interrupt?
        sprintf(stringBuilder, "TIMER0 unexpected interrupt: 0x%X", callers);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
}

//non-delayed input
void ISR_TIMER2_A(void)
{
    uint32_t callers = TimerIntStatus(TIMER2_BASE, true) & (~TIMER_CAPB_EVENT);   //determines what triggered the interrupt
    TimerIntClear(TIMER2_BASE, callers);    //clears the interrupt flags

    if(callers & TIMER_CAPA_EVENT){  //expected interrupt?
        UARTPrint(UART0_BASE, "\r\n----------\r\n");
        //if new edge was captured
        //if(measuring)
        if(timestamp == -1)
            timestamp = TimerValueGet(TIMER2_BASE, TIMER_A);
        else {
            timestamp = -1;
            offset = 0;
            UARTPrint(UART0_BASE, "\r\n> ERROR: missed edge on B\r\n\r\n");
        }
    }
    else {
        sprintf(stringBuilder, "TIMER A unexpected interrupt: 0x%X", callers);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
    if(callers - TIMER_CAPA_EVENT) {  //unexpected interrupt?
        sprintf(stringBuilder, "TIMER A unexpected interrupt: 0x%X", callers);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
}

//delayed input
void ISR_TIMER2_B(void)
{
    uint32_t callers = TimerIntStatus(TIMER2_BASE, true) & (~TIMER_CAPA_EVENT);   //determines what triggered the interrupt
    TimerIntClear(TIMER2_BASE, callers);    //clears the interrupt flags

    if(callers & TIMER_CAPB_EVENT){  //expected interrupt?
        //if(measuring)
        if(timestamp == -1) {
            timestamp = -1;
            offset = 0;
            UARTPrint(UART0_BASE, "\r\n> ERROR: missed edge on A\r\n\r\n");
        }
        else {
            uint32_t timestampB = TimerValueGet(TIMER2_BASE, TIMER_B); //saves timestamp
            if(timestampB + offset > timestamp) {   //checks if B has been captured after A
                uint8_t cheksum = 78, out[84] = {'$', 'G', 'P', 'D', 'L', 'Y', ','};
                uint_fast8_t outCount = 7;
                int32_t num = ((double)(offset + timestampB - timestamp)) / (INPUT_CAPTURE_CLOCK_FREQUENCY / 1000000000.0);

                uint8_t digits[11];
                int_fast8_t count = 0;
                do {
                    digits[count++] = '0' + (num % 10);
                    num /= 10;
                }while(num);
                for(count--; count >= 0; count--) {
                    out[outCount++] = digits[count];
                    cheksum ^= digits[count];
                }
                out[outCount++] = '*';
                out[outCount] = '\0';
                UARTPrint(UART0_BASE, out);
                UARTPrint_i32(UART0_BASE, cheksum);
                UARTPrint(UART0_BASE, "\r\n");
                timestamp = -1;
                offset = 0;
                //measuring--;
            }
            else {
                timestamp = -1;
                offset = 0;
                UARTPrint(UART0_BASE, "\r\n> ERROR: B predates A\r\n\r\n");
                TimerIntClear(TIMER2_BASE, TIMER_CAPA_EVENT & TIMER_CAPB_EVENT);
            }
        }
    }
    else {
        sprintf(stringBuilder, "TIMER B unexpected interrupt 1: callers->0x%X", callers);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
    if(callers - TIMER_CAPB_EVENT) {  //unexpected interrupt?
        sprintf(stringBuilder, "TIMER B unexpected interrupt 2: 0x%X", callers);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
}

void latencyInit()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //enables GPIOA
    //TIMER0 is needed, because timers configured for input capture doesn't raise overflow interrupts
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   //enables TIMER 0, which needs to enabled in order to make GPTMSYNC accessible
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);   //enables TIMER 2

    //waits until peripheries are ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2));

    //configures timers for two-signal edge-separation
    GPIOPinConfigure(GPIO_PA4_T2CCP0);   //sets PA4 as the input capture input for TIMER2 TIMER A
    GPIOPinConfigure(GPIO_PA5_T2CCP1);   //sets PA5 as the input capture input for TIMER2 TIMER B
    GPIOPinTypeTimer(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5); //configures the pins
    TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);  //TIMER0 is clocked from the system clock
    TimerClockSourceSet(TIMER2_BASE, TIMER_CLOCK_SYSTEM);  //TIMER2 is clocked from the system clock
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC_UP)); //configures TIMER0
    TimerConfigure(TIMER2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP)); //configures TIMER2 TIMER A/B
    TimerControlEvent(TIMER2_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);   //configures Timer A to capture rising edges
    TimerControlEvent(TIMER2_BASE, TIMER_B, TIMER_EVENT_POS_EDGE);   //configures Timer B to capture rising edges
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //enables overflow interrupt on TIMER0 A
    TimerIntEnable(TIMER2_BASE, (TIMER_CAPB_EVENT | TIMER_CAPA_EVENT));   //enables interrupt for edge capture on TIMER2 TIMER A/B and for overflow on TIMER2 TIMER A
    TimerLoadSet(TIMER0_BASE, TIMER_A, 0xFFFF);
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 0xFF);
    TimerLoadSet(TIMER2_BASE, TIMER_BOTH, 0xFFFF);
    TimerPrescaleSet(TIMER2_BASE, TIMER_BOTH, 0xFF);
    TimerEnable(TIMER2_BASE, TIMER_BOTH);
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerSynchronize(TIMER0_BASE, (TIMER_0A_SYNC | TIMER_2A_SYNC | TIMER_2B_SYNC));  //Synchronizes TIMER0 A and TIMER2 A and B with each other
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER2A);
    IntEnable(INT_TIMER2B);
}
