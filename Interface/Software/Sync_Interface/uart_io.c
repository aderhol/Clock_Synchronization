#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "uart_io.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/gpio.h" //GPIO
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"    //interrupt defines (eg. INT_UART0)
#include "driverlib/sysctl.h" //System Control
#include "command.h"

#define RINGBUFFER_SIZE 1024
#define ringptrinc(X) ((X + 1) % RINGBUFFER_SIZE)

typedef struct {
    int32_t writePtr;
    int32_t readPtr;
    uint8_t data[RINGBUFFER_SIZE];
} ringbuffer;

bool show_gps1 = true, show_line = false;

static ringbuffer uartwriteBuffer_0, uartreadBuffer_0, uartwriteBuffer_4, uartreadBuffer_4, uartwriteBuffer_6, uartreadBuffer_6;

extern uint32_t SYS_CLK_FREQ_ACTUAL;

static inline ringbuffer* baseToBuffer(uint32_t base, bool write)
{
    switch (base){
    case UART0_BASE:
        if(write)
            return &uartwriteBuffer_0;
        else
            return &uartreadBuffer_0;
        break;
    case UART4_BASE:
        if(write)
            return &uartwriteBuffer_4;
        else
            return &uartreadBuffer_4;
        break;
    case UART6_BASE:
        if(write)
            return &uartwriteBuffer_6;
        else
            return &uartreadBuffer_6;
        break;
    default:
        return NULL;
        break;
    }
}

void UARTPrint(uint32_t base, const uint8_t* message)
{
    while((*message) != '\0')
        UARTPutch(base, *(message++));
}

void UARTPrint_i32(uint32_t base, int32_t num) {
    unsigned char out[11];
    int_fast8_t count = 0;
    if(num < 0) {
        UARTPutch(base, '-');
        num *= -1;
    }
    do {
        out[count++] = '0' + (num % 10);
        num /= 10;
    }while(num);
    for(count--; count >= 0; count--)
        UARTPutch(base, out[count]);
}

void UARTInit(void)
{//4-es, R> k0, T> k1

    //enable UART pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);        //USB
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);        //GPS_1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);        //line

    //enables UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART6)); //wait for UART to be ready

    //sets pins as UART pins
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinConfigure(GPIO_PK0_U4RX);
    GPIOPinConfigure(GPIO_PK1_U4TX);
    GPIOPinConfigure(GPIO_PP0_U6RX);
    GPIOPinConfigure(GPIO_PP1_U6TX);

    //configures UART pins (input/output)
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeUART(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeUART(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //configure the UART modules
    //UART0(USB):   baud-rate: 115200,  8-bit, 1 STOP-bit, no parity-bit
    //UART4(GPS_1): baud-rate: 9600,    8-bit, 1 STOP-bit, no parity-bit
    //UART6(line):  baud-rate: 115200,  8-bit, 1 STOP-bit, no parity-bit
    //AND enables UART
    UARTConfigSetExpClk(UART0_BASE, SYS_CLK_FREQ_ACTUAL, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTConfigSetExpClk(UART4_BASE, SYS_CLK_FREQ_ACTUAL, 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTConfigSetExpClk(UART6_BASE, SYS_CLK_FREQ_ACTUAL, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTFIFOEnable(UART0_BASE); //enables FIFOs
    UARTFIFOEnable(UART4_BASE); //enables FIFOs
    UARTFIFOEnable(UART6_BASE); //enables FIFOs
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX2_8, UART_FIFO_RX4_8);
    UARTFIFOLevelSet(UART4_BASE, UART_FIFO_TX2_8, UART_FIFO_RX4_8);
    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX2_8, UART_FIFO_RX4_8);
    UARTIntEnable(UART0_BASE, (UART_INT_RT | UART_INT_TX | UART_INT_RX)); //unmasks Receive Timeout and receive FIFO interrupts
    UARTIntEnable(UART4_BASE, (UART_INT_RT | UART_INT_TX | UART_INT_RX));
    UARTIntEnable(UART6_BASE, (UART_INT_RT | UART_INT_TX | UART_INT_RX)); //unmasks Receive Timeout and receive FIFO interrupts
    IntEnable(INT_UART0);   //enables unmasked UART interrupts
    IntEnable(INT_UART4);
    IntEnable(INT_UART6);

    uartreadBuffer_0.writePtr = 0;
    uartreadBuffer_0.readPtr = 0;
    uartwriteBuffer_0.writePtr = 0;
    uartwriteBuffer_0.readPtr = 0;

    uartreadBuffer_4.writePtr = 0;
    uartreadBuffer_4.readPtr = 0;
    uartwriteBuffer_4.writePtr = 0;
    uartwriteBuffer_4.readPtr = 0;

    uartreadBuffer_6.writePtr = 0;
    uartreadBuffer_6.readPtr = 0;
    uartwriteBuffer_6.writePtr = 0;
    uartwriteBuffer_6.readPtr = 0;
}

void UARTPutch(uint32_t base, uint8_t ch) {
    ringbuffer* buffer = baseToBuffer(base, true);
    if((buffer->writePtr != buffer->readPtr) || !UARTCharPutNonBlocking(base, ch)) {
        //blocks while buffer is full
        if(ringptrinc(buffer->writePtr) == buffer->readPtr) {
            while (ringptrinc(buffer->writePtr) == buffer->readPtr);
            if((buffer->writePtr == buffer->readPtr) && UARTCharPutNonBlocking(base, ch))
                return;
        }
        buffer->data[buffer->writePtr] = ch;
        buffer->writePtr = ringptrinc(buffer->writePtr);
    }
}

bool uartHasData(uint32_t base) {
    ringbuffer* buffer = baseToBuffer(base, false);
    return buffer->writePtr != buffer->readPtr;
}

uint8_t UARTGetch(uint32_t base) {
    ringbuffer* buffer = baseToBuffer(base, false);
    uint8_t ret;
    //blocks while buffer is empty
    while (buffer->writePtr == buffer->readPtr);
    ret = buffer->data[buffer->readPtr];
    buffer->readPtr = ringptrinc(buffer->readPtr);
    return ret;
}

void ISR_UART0(void)
{
    uint32_t callers = UARTIntStatus(UART0_BASE, true);  //determines what triggered the interrupt
    UARTIntClear(UART0_BASE, callers);  //clears the interrupt flags

    if(callers & (UART_INT_RT | UART_INT_RX)){
        while(UARTCharsAvail(UART0_BASE)){
            if(ringptrinc(uartreadBuffer_0.writePtr) != uartreadBuffer_0.readPtr) { //if the buffer is not full
                uartreadBuffer_0.data[uartreadBuffer_0.writePtr] = UARTCharGetNonBlocking(UART0_BASE);
                uartreadBuffer_0.writePtr = ringptrinc(uartreadBuffer_0.writePtr);
            }
            else
                UARTCharGetNonBlocking(UART0_BASE);
        }
    }

    if(callers & UART_INT_TX) {
        while((uartwriteBuffer_0.readPtr != uartwriteBuffer_0.writePtr) && UARTCharPutNonBlocking(UART0_BASE, uartwriteBuffer_0.data[uartwriteBuffer_0.readPtr]))
            uartwriteBuffer_0.readPtr = ringptrinc(uartwriteBuffer_0.readPtr);
    }
}

void ISR_UART4(void)
{
    uint32_t callers = UARTIntStatus(UART4_BASE, true);  //determines what triggered the interrupt
    UARTIntClear(UART4_BASE, callers);  //clears the interrupt flags

    if(callers & (UART_INT_RT | UART_INT_RX)){
        while(UARTCharsAvail(UART4_BASE)){
            //UARTPutch(UART0_BASE, UARTCharGetNonBlocking(UART4_BASE));
            if(ringptrinc(uartreadBuffer_4.writePtr) != uartreadBuffer_4.readPtr) { //if the buffer is not full
                uartreadBuffer_4.data[uartreadBuffer_4.writePtr] = UARTCharGetNonBlocking(UART4_BASE);
                uartreadBuffer_4.writePtr = ringptrinc(uartreadBuffer_4.writePtr);
            }
            else
                UARTCharGetNonBlocking(UART4_BASE);
        }
    }

    if(callers & UART_INT_TX) {
        while((uartwriteBuffer_4.readPtr != uartwriteBuffer_4.writePtr) && UARTCharPutNonBlocking(UART4_BASE, uartwriteBuffer_4.data[uartwriteBuffer_4.readPtr]))
            uartwriteBuffer_4.readPtr = ringptrinc(uartwriteBuffer_4.readPtr);
    }
}

//line
void ISR_UART6(void)
{
    uint32_t callers = UARTIntStatus(UART6_BASE, true);  //determines what triggered the interrupt
    UARTIntClear(UART6_BASE, callers);  //clears the interrupt flags

    if(callers & (UART_INT_RT | UART_INT_RX)){
        while(UARTCharsAvail(UART6_BASE)){
            if(ringptrinc(uartreadBuffer_6.writePtr) != uartreadBuffer_6.readPtr) { //if the buffer is not full
                uartreadBuffer_6.data[uartreadBuffer_6.writePtr] = UARTCharGetNonBlocking(UART6_BASE);
                uartreadBuffer_6.writePtr = ringptrinc(uartreadBuffer_6.writePtr);
            }
            else
                UARTCharGetNonBlocking(UART6_BASE);
        }
    }

    if(callers & UART_INT_TX) {
        while((uartwriteBuffer_6.readPtr != uartwriteBuffer_6.writePtr) && UARTCharPutNonBlocking(UART6_BASE, uartwriteBuffer_6.data[uartwriteBuffer_6.readPtr]))
            uartwriteBuffer_6.readPtr = ringptrinc(uartwriteBuffer_6.readPtr);
    }
}

typedef enum {
    listen,     //start character has not been received
    collect,    //start character has been received
    cr,         //carriage return has been received
}GPS_NMEA_MessageState;

void UARTTransferGPSData(void)
{
    static uint8_t message[83];
    static int_fast8_t count = 0;
    static GPS_NMEA_MessageState state = listen;

    while(uartHasData(UART4_BASE)) {
        uint8_t ch = UARTGetch(UART4_BASE);
        switch(state) {
        case listen:
            if(ch == '!' || ch == '$') {
                state = collect;
                message[0] = ch;
                count = 1;
            }
            break;
        case collect:
            if(ch != '!' && ch != '$' && ch != '\r' && ch != '\n' && count < 81) //if a simple message character is received and the NMEA defined character count is not exceeded
                message[count++] = ch;
            else if (ch == '\r' && count < 82) {    //if a carriage return is received
                state = cr;
                message[count++] = ch;
            }
            else if (ch == '!' || ch == '$') {  //if beginning of new message character received
                state = collect;
                message[0] = ch;
                count = 1;
            }
            else {  //not NMEA conform message detected
                state = listen;
                count = 0;
            }
            break;
        case cr:
            if(ch == '\n') { //if closing line-feed received
                message[count++] = '\n';
                message[count] = '\0';
                if(show_gps1){
                    UARTPrint(UART0_BASE, message);
                    UARTPrint(UART6_BASE, message);
                }
            }
            state = listen;
            count = 0;
            break;
        }
    }
}

void UARTTransferLineData(void)
{
    static uint8_t message[101];
    static int_fast8_t count = 0;
    static bool command = false;

    while(uartHasData(UART6_BASE)) {
        uint8_t ch = UARTGetch(UART6_BASE);
        if(count == 0 && ch == '>')
            command = true;
        else{
            message[count++] = ch;
            if(command){
                if(ch == '\n'){
                    if(message[count - 2] == '\r')
                        message[count - 2] = '\0';
                    else
                        message[count - 1] = '\0';
                    execute(message, UART6_BASE);
                    count = 0;
                    command = false;
                }
                else if(count == 100){
                    UARTPrint(UART6_BASE, "Invalid command!\r\n");
                    count = 0;
                    command = false;
                }
            }
            else if(ch == '\n' || count == 100){
                message[count++] = '\0';
                if(show_line)
                    UARTPrint(UART0_BASE, message);
                count = 0;
            }
        }
    }
}
