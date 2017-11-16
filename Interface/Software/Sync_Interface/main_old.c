#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h" //System Control
#include "driverlib/uart.h" //UART
#include "driverlib/gpio.h" //GPIO
#include "uart_io.h"  //UART functionalities
#include "pwm.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/interrupt.h"    //interrupt API
#include "inc/hw_ints.h"    //interrupt defines (eg. INT_UART0)
#include "command.h"
#include "driverlib/fpu.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"    //timer

#define SYS_CLK_FREQ 120000000
uint32_t SYS_CLK_FREQ_ACTUAL;

static void InterruptConfigFaultISR(const uint8_t* message)
{
    UARTPrint(UART0_BASE, "\r\n\r\nHard Fault: ");
    UARTPrint(UART0_BASE, message);
    UARTPrint(UART0_BASE, "\r\n");
    while(1);
}


static char stringBuilder[101];

void init(void)
{
    //set the System Clock Frequency to 120 MHz
    SYS_CLK_FREQ_ACTUAL = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), SYS_CLK_FREQ);
    FPUEnable();

    UARTInit();
    PWMInit();

    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //enables GPIOA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   //enables TIMER 0, which needs to enabled in order to make GPTMSYNC accessible
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);   //enables TIMER 2

    //wait until peripheries are ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2));

   // GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_1);
    //GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);


    //configures timers for two-signal edge-separation
    GPIOPinConfigure(GPIO_PA4_T2CCP0);   //sets PA4 as the input capture input for TIMER2 TIMER A
    GPIOPinConfigure(GPIO_PA5_T2CCP1);   //sets PA5 as the input capture input for TIMER2 TIMER B
    GPIOPinTypeTimer(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5); //configures the pins
    TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);  //TIMER0 is clocked from the system clock
    TimerClockSourceSet(TIMER2_BASE, TIMER_CLOCK_SYSTEM);  //TIMER2 is clocked from the system clock
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC_UP)); //configures TIMER0
    TimerConfigure(TIMER2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP)); //configures TIMER2 TIMER A/B
    TimerControlEvent(TIMER2_BASE, TIMER_BOTH, TIMER_EVENT_POS_EDGE);   //configures timers to capture rising edges
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

    IntMasterEnable();  //enables unmasked interrupts
}

#define INPUT_CAPTURE_BUFFER_SIZE 100 //has to be at least 2, the state machine doesn't work otherwise
#define INPUT_CAPTURE_TIMER_WIDTH 16777216 //24 bit wide -> 2^24 = 16,777,216
#define INPUT_CAPTURE_CLOCK_FREQUENCY SYS_CLK_FREQ
static int_fast32_t offset = 0, inputCaptureBuffer[INPUT_CAPTURE_BUFFER_SIZE], discarded1 = 0, discarded2 = 0, sum = 0, N = 0;
static bool captured = false, inputCaptureBuffer_empty = true, inputCaptureBuffer_full = false, discarding1Active = false;
static int_fast8_t readPtr = 0, writePtr = 0;

int main(void)
{

    init();

    UARTPrint(UART0_BASE, "Configured!\r\n\r\n> ");
    while(1)
    {
        CommandInterpreter();
    }
}


static inline void evaulateInputCapture(void)
{
    static int i = 0;
    //i++;
    //while(i == 50);
    UARTPrint(UART0_BASE, "\r\n delay: ");
    if(N)
        UARTPrint_i32(UART0_BASE, (((double) sum / N) / (INPUT_CAPTURE_CLOCK_FREQUENCY / 1000000000.0)));
    else
        UARTPrint(UART0_BASE, " ERROR");
    UARTPrint(UART0_BASE, " ns\r\n N: ");
    UARTPrint_i32(UART0_BASE, N);
    UARTPrint(UART0_BASE, " data points\r\n\r\n> ");
}

void pulse(void)
{
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0xF);
    SysCtlDelay(250);
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
}

static inline void inputCaptureBuffer_push(int_fast32_t time)
{
    if(inputCaptureBuffer_empty) {
        //B:0
        if(discarded1) {
            inputCaptureBuffer[writePtr] = time;
            writePtr = (writePtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;  //increment writePtr
            //the buffer was empty, cannot be full now
            inputCaptureBuffer_empty = false;
            return;
        }
        else {
            inputCaptureBuffer[writePtr] = time;
            writePtr = (writePtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;  //increment writePtr
            //the buffer was empty, cannot be full now
            inputCaptureBuffer_empty = false;
            return;
        }
    }
    else{
        if(inputCaptureBuffer_full) {
            //B:F
            if(discarded1) {
                if(discarded2) {
                    discarded2++;
                    return;
                }
                else
                {
                    if (discarding1Active) {
                        discarded1++;
                        return;
                    }
                    else {
                        discarded2++;
                        return;
                    }
                }
            }
            else {
                discarded1++;
                discarding1Active = true;
                return;
            }
        }
        else {
            //B:X
            if(discarded1){
                if(discarding1Active) {
                    discarded1++;
                    return;
                }
                else {
                    inputCaptureBuffer[writePtr] = time;
                    writePtr = (writePtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;  //increment writePtr
                    if(writePtr == readPtr) //is the buffer full?
                        inputCaptureBuffer_full = true;
                    return;
                }
            }
            else {
                inputCaptureBuffer[writePtr] = time;
                writePtr = (writePtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;  //increment writePtr
                if(writePtr == readPtr) //is the buffer full?
                    inputCaptureBuffer_full = true;
                return;
            }
        }
    }
}

//returning -1 means no corresponding timestamp is recorded, -2 means the buffer is empty
static inline const int32_t inputCaptureBuffer_pop()
{
    if(inputCaptureBuffer_empty) {
        //B:0
        if(discarded1) {
            discarded1--;
            return -1;
        }
        else {
            return -2;
        }
    }
    else{
        if(inputCaptureBuffer_full) {
            //B:F
            if(discarded1) {
                if(discarded2) {
                    discarded1--;
                    if(discarded1 == 0) {
                        discarded1 = discarded2;
                        discarded2 = 0;
                        discarding1Active = true;
                    }
                    return -1;
                }
                else
                {
                    if (discarding1Active) {
                        int32_t temp = inputCaptureBuffer[readPtr];
                        readPtr = (readPtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;
                        inputCaptureBuffer_full = false;
                        //buffer was full, cannot be empty now
                        return temp;
                    }
                    else {
                        discarded1--;
                        return -1;
                    }
                }
            }
            else {
                int32_t temp = inputCaptureBuffer[readPtr];
                readPtr = (readPtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;    //increment readPtr
                inputCaptureBuffer_full = false;
                return temp;
            }
        }
        else {
            //B:X
            if(discarded1){
                if(discarding1Active) {
                    int32_t temp = inputCaptureBuffer[readPtr];
                    readPtr = (readPtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;
                    if(writePtr == readPtr) {   //is the buffer empty?
                        inputCaptureBuffer_empty = true;
                        discarding1Active = false;
                    }
                    return temp;
                }
                else {
                    discarded1--;
                    return -1;
                }
            }
            else {
                int32_t temp = inputCaptureBuffer[readPtr];
                readPtr = (readPtr + 1) % INPUT_CAPTURE_BUFFER_SIZE;
                if(writePtr == readPtr) //is the buffer empty?
                    inputCaptureBuffer_empty = true;
                return temp;
            }
        }
    }
}

static inline void resetInputCapture(void)
{
    captured = false;
    inputCaptureBuffer_empty = true;
    readPtr = writePtr = 0;
    inputCaptureBuffer_full = discarding1Active = false;
    discarded1 = discarded2 = 0;
    offset = 0;
    sum = 0;
    N = 0;
}

void ISR_TIMER0_A(void)
{
    uint32_t callers = TimerIntStatus(TIMER0_BASE, true);   //determines what triggered the interrupt
    TimerIntClear(TIMER0_BASE, callers);    //clears the interrupt flags

    if(callers & TIMER_TIMA_TIMEOUT) { //timer timeout
        if(!inputCaptureBuffer_empty || discarded1 || N) {  //other than base case or there is data to evaulate?
            if(!captured) { //last edge came too long ago
                evaulateInputCapture();
                resetInputCapture();
            }
            else {
                offset += INPUT_CAPTURE_TIMER_WIDTH;
                if(offset + INPUT_CAPTURE_TIMER_WIDTH - 1 < 0) { //offset + timestamp may overflow?
                    evaulateInputCapture();
                    resetInputCapture();
                }
                else
                    captured = false;
            }
        }
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

void ISR_TIMER2_A(void)
{
    uint32_t callers_full = TimerIntStatus(TIMER2_BASE, true), callers = callers_full & (~TIMER_CAPB_EVENT);   //determines what triggered the interrupt
    TimerIntClear(TIMER2_BASE, callers);    //clears the interrupt flags

    if(callers & TIMER_CAPA_EVENT){  //expected interrupt?
        //if new edge was captured
        if(inputCaptureBuffer_empty && (discarded1 == 0) && (callers_full & TIMER_CAPB_EVENT) && (TimerValueGet(TIMER2_BASE, TIMER_A) > TimerValueGet(TIMER2_BASE, TIMER_B)))
            TimerIntClear(TIMER2_BASE, TIMER_CAPB_EVENT);
        inputCaptureBuffer_push(offset + TimerValueGet(TIMER2_BASE, TIMER_A));
        captured = true;
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

void ISR_TIMER2_B(void)
{
    uint32_t callers_full = TimerIntStatus(TIMER2_BASE, true), callers = callers_full  & (~(TIMER_CAPA_EVENT | TIMER_TIMA_TIMEOUT));   //determines what triggered the interrupt
    TimerIntClear(TIMER2_BASE, callers);    //clears the interrupt flags

    if(callers & TIMER_CAPB_EVENT){  //expected interrupt?
        int_fast32_t phaseA = inputCaptureBuffer_pop();
        switch(phaseA) {
        case -1 :   //edge is captured on A, but timestamp is not recorded for the capture
            break;
        case -2:    //edge on B captured without a corresponding edge on A
            evaulateInputCapture();
            resetInputCapture();
            break;
        default: {
            int_fast32_t temp = offset + TimerValueGet(TIMER2_BASE, TIMER_B) - phaseA;
            sum += temp;
            if(sum < 0) {
                sum -= temp;
                evaulateInputCapture();
                resetInputCapture();
            }
            else
                N++;
            /*evaulateInputCapture();
            resetInputCapture();*/
        } break;
        }
    }
    else {
        sprintf(stringBuilder, "TIMER B unexpected interrupt 1: callers->0x%X callers_full->0x%X", callers, callers_full);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
    if(callers - TIMER_CAPB_EVENT) {  //unexpected interrupt?
        sprintf(stringBuilder, "TIMER B unexpected interrupt 2: 0x%X", callers);
        InterruptConfigFaultISR(stringBuilder); //enters fault handler
    }
}
