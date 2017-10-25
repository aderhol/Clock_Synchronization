#include <stdint.h>
#include <errno.h>
#include "command.h"
#include <string.h>
#include <stdbool.h>
#include "uart_io.h"
#include "latency.h"

extern void pulse(void);
extern void InterruptConfigFaultISR(const uint8_t*);

#define COMMAND_BUFFER_SIZE 10

const static char* commands[] = {"help"/*, "measure"*/, "pulse", "pwm", "sitrep"};
const static char* usage[] = {" help"/*, " measure {number of measurements to be performed (1-127)}"*/, " pulse", " PWM set [PWM-clock-ticks (period)] [PWM-clock-ticks (pulse-width)]\r\n         period [PWM-clock-ticks]\r\n         pulse-width [PWM-clock-ticks]\r\n     get", " sitrep"};
enum {HELP/*, MEASURE*/, PULSE, PWM, SITREP};

static void getCommands(uint32_t);
static uint8_t* FetchCommand(void);
static void execute(uint8_t*);

static uint8_t buffer[COMMAND_BUFFER_SIZE][101], readPtr = 0, writePtr = 0;
static bool empty = true;

static inline bool push(uint8_t* command)
{
    if(readPtr != writePtr || empty){
        strcpy(buffer[writePtr], command);
        writePtr = (writePtr + 1) % COMMAND_BUFFER_SIZE;
        empty = false;
        return true;
    }
    else
        return false;
}

static inline const uint8_t* pop()
{
    if (empty)
        return NULL;
    else {
        uint8_t* temp = buffer[readPtr];
        readPtr = (readPtr + 1) % COMMAND_BUFFER_SIZE;
        if(readPtr == writePtr)
            empty = true;
        return temp;
    }
}

static void getCommands(uint32_t base)
{
    static uint8_t line[101];
    static uint_fast8_t count = 0;

    while(uartHasData(base)){
        uint_fast8_t ch = UARTGetch(base);
        if(ch == '\n'){
            if(line[count - 1] == '\r')
                line[count - 1] = '\0';
            else
                line[count] = '\0';

            count = 0;

            push(line);
        }
        else
            if(ch != '\0') {
                if(count == 100)
                    InterruptConfigFaultISR("Command overrun");
                if(ch == '\b')
                    count = (count == 0) ? 0 : count - 1;
                else
                    line[count++] = ch;
            }
    }
}

static inline  uint8_t* FetchCommand(void) {
    static uint8_t holster[101];
    const uint8_t* temp = pop();
    if(temp == NULL)
        return NULL;
    else {
        strcpy(holster, temp);
        return holster;
    }
}

void CommandInterpreter(void)
{
    //read commands
    getCommands(UART0_BASE);

    //execute commands
    uint8_t* command;
    while((command = FetchCommand()) != NULL)
        execute(command);
}

void execute(uint8_t* command) {

    if( command[0] == '$') {  //if NMEA message
        UARTPrint(UART4_BASE, command);
        UARTPrint(UART4_BASE, "\r\n");
        return;
    }

    char* tokens[15];
    int_fast8_t count = 0; //number of tokens

    //to lower case
    for(tokens[0] = command; *tokens[0] != '\0'; tokens[0]++)
        *tokens[0] = tolower(*tokens[0]);

    //tokenization
    while((tokens[count] = strtok(count ? NULL : command, " \r\n")) != NULL && count < (sizeof(tokens) / sizeof(tokens[0]) - 1))
        count++;
    if(count > 0) {
        if(count == (sizeof(tokens) / sizeof(tokens[0]) - 1) && tokens[count] != NULL && (count++, strtok(NULL, " \r\n") != NULL)) //to much tokens?
            UARTPrint(UART0_BASE, " Invalid command!\r\n");
        else {
            int_fast8_t start = 0, end = sizeof(commands) / sizeof(commands[0]) - 1, cmd = -1;

            //binary search
            do {
                int cmp;
                int_fast8_t ref;

                ref = (start + end) / 2;
                cmp = strcmp(tokens[0], commands[ref]);
                if(cmp == 0)
                    cmd = ref;
                else
                    if(cmp < 0)
                        end = ref - 1;
                    else
                        start = ref + 1;
            }while(cmd == -1 && start <= end);

            if(cmd != -1) {
                switch(cmd) {
                case HELP: //help
                    for(start = 0; start < sizeof(usage) / sizeof(usage[0]); start++) {
                        //UARTPrint(UART0_BASE, "");
                        UARTPrint(UART0_BASE, usage[start]);
                        UARTPrint(UART0_BASE, "\r\n");
                        if(count > 1)
                            UARTPrint(UART0_BASE, "\r\n Syntax error!\r\n Usage: sitrep\r\n");
                    }
                    break;
                case PWM: //PWM
                    if(strcmp(tokens[1], "set") == 0 && count == 4){
                        if(strcmp(tokens[2], "period") == 0) {
                            char* endptr;
                            long period;
                            errno = 0;
                            period = strtol(tokens[3], &endptr, 10);
                            if(endptr == tokens[3] || errno == ERANGE) {
                                errno = 0;
                                UARTPrint(UART0_BASE, " Invalid command!\r\n");
                            }
                            else {
                                PWMPeriodSet(period);
                                UARTPrint(UART0_BASE, " PWM period set to ");
                                UARTPrint(UART0_BASE, tokens[3]);
                                UARTPrint(UART0_BASE, " PWM clock-ticks.\r\n");
                            }
                            break;
                        }
                        if(strcmp(tokens[2], "pulse-width") == 0) {
                            char* endptr;
                            long pw;
                            errno = 0;
                            pw = strtol(tokens[3], &endptr, 10);
                            if(endptr == tokens[3] || errno == ERANGE) {
                                errno = 0;
                                UARTPrint(UART0_BASE, " Invalid command!\r\n");
                            }
                            else {
                                PWMPulseSet(pw);
                                UARTPrint(UART0_BASE, " PWM pulse-width set to ");
                                UARTPrint(UART0_BASE, tokens[3]);
                                UARTPrint(UART0_BASE, " PWM clock-ticks.\r\n");
                            }
                            break;
                        }
                        char* endptr;
                        long period, pw;
                        errno = 0;
                        period = strtol(tokens[2], &endptr, 10);
                        if(endptr == tokens[2] || errno == ERANGE) {
                            errno = 0;
                            UARTPrint(UART0_BASE, " Invalid command!\r\n");
                            break;
                        }

                        errno = 0;
                        pw = strtol(tokens[3], &endptr, 10);
                        if(endptr == tokens[3] || errno == ERANGE) {
                            errno = 0;
                            UARTPrint(UART0_BASE, " Invalid command!\r\n");
                            break;
                        }
                        PWMPeriodSet(period);
                        PWMPulseSet(pw);
                        UARTPrint(UART0_BASE, " PWM period set to ");
                        UARTPrint(UART0_BASE, tokens[2]);
                        UARTPrint(UART0_BASE, " PWM clock-ticks.\r\n");
                        UARTPrint(UART0_BASE, " PWM pulse-width set to ");
                        UARTPrint(UART0_BASE, tokens[3]);
                        UARTPrint(UART0_BASE, " PWM clock-ticks.\r\n");
                    }
                    else {
                        if(strcmp(tokens[1], "get") == 0 && count == 2) {
                            UARTPrint(UART0_BASE, " Period: ");
                            UARTPrint_i32(UART0_BASE, PWMPeriodeGet());
                            UARTPrint(UART0_BASE, " PWM clock-ticks\r\n Pulse-Width: ");
                            UARTPrint_i32(UART0_BASE, PWMPulseGet());
                            UARTPrint(UART0_BASE, " PWM clock-ticks\r\n");
                        }
                        else
                            UARTPrint(UART0_BASE, " Invalid command!\r\n");
                    }
                    break;
                case SITREP: //sitrep
                    UARTPrint(UART0_BASE, " Awaiting instructions.\r\n");
                    if(count > 1)
                        UARTPrint(UART0_BASE, "\r\n Syntax error!\r\n Usage: sitrep\r\n");
                    break;
                case PULSE: //pulse
                    pulse();
                    if(count > 1)
                        UARTPrint(UART0_BASE, "\r\n Syntax error!\r\n Usage: pulse\r\n");
                    break;
                /*case MEASURE: //measure
                    if(count == 1)
                        measureLatency(1);
                    else if(count == 2) {
                        char* endptr;
                        long c;
                        c = strtol(tokens[1], &endptr, 10);
                        if(c <= INT8_MAX && c > 0)
                            measureLatency((int8_t) c);
                        else
                            if(c <= 0) {
                                UARTPrint(UART0_BASE, "\r\n Syntax error!\r\n Usage:");
                                UARTPrint(UART0_BASE, usage[MEASURE]);
                                UARTPrint(UART0_BASE, "\r\n");
                            }
                            else
                                UARTPrint(UART0_BASE, "\r\n Specified number is too large!\r\n Maximum is: 127\r\n");
                    }
                    else {
                        UARTPrint(UART0_BASE, "\r\n Syntax error!\r\n Usage:");
                        UARTPrint(UART0_BASE, usage[MEASURE]);
                        UARTPrint(UART0_BASE, "\r\n");
                    }
                    break;*/
                default:
                    UARTPrint(UART0_BASE, " Unimplemented command!\r\n");
                    break;
                }
            }
            else
                UARTPrint(UART0_BASE, " Invalid command!\r\n");

        }
    }
    //UARTPrint(UART0_BASE, "\r\n\r\n> ");
}
