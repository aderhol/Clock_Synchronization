#include <stdint.h>
#include "UART_controller.h"
#include "uart_io.h"
#include "command.h"
#include "inc/hw_memmap.h"

static bool CommandInterpreterEnable = true, UARTTransferGPSDataEnable = true, UARTTransferLineDataEnable = true, bridgePC2LineEnable = false, bridgeGPS2LineEnable = false;
static bool bridged = false;

static void bridgePC2Line_bridge(void);
static void bridgeGPS22Line_bridge(void);

void UARTHandler(void)
{
    if(CommandInterpreterEnable)
        CommandInterpreter();
    if(UARTTransferGPSDataEnable)
        UARTTransferGPSData();
    if(UARTTransferLineDataEnable)
        UARTTransferLineData();
    if(bridgePC2LineEnable)
        bridgePC2Line_bridge();
    else if(bridgeGPS2LineEnable)
        bridgeGPS22Line_bridge();
}

bool bridgePC2Line(void){
    if(bridged)
        return 0;
    bridged = true;
    CommandInterpreterEnable = false;
    UARTDisableBase(UART0_BASE);
    UARTClearBuffers(UART0_BASE);
    UARTTransferGPSDataEnable = false;
    UARTTransferLineDataEnable = false;
    UARTDisableBase(UART6_BASE);
    UARTClearBuffers(UART6_BASE);
    bridgePC2LineEnable = true;
    return 1;
}

void unBridgePCFromLine(void) {
    if(!bridgePC2LineEnable)
        return;
    bridgePC2LineEnable = false;
    UARTClearBuffers(UART0_BASE);
    UARTEnableBase(UART0_BASE);
    CommandInterpreterEnable = true;
    UARTClearBuffers(UART6_BASE);
    UARTEnableBase(UART6_BASE);
    UARTTransferLineDataEnable = true;
    UARTTransferGPSDataEnable = true;
    bridged = false;
    UARTPrint(UART0_BASE, "\r\n PC and line is now UNbridged.\r\n");
}

void bridgePC2Line_bridge(void){
    static int statePC = 0;
    static int stateLine = 0;
    uint8_t ch;

    while(UARTGetchElev_NB(UART0_BASE, &ch)) {
        switch(ch){
        case '\'':
            statePC = 1;
            break;
        case '\"':
            statePC = statePC == 1 ? 2 : 0;
            break;
        case '+':
            statePC = statePC == 2 ? 3 : 0;
            break;
        case '!':
            statePC = statePC == 3 ? 4 : 0;
            break;
        case '%':
            statePC = statePC == 4 ? 5 : 0;
            break;
        case '/':
            statePC = statePC == 5 ? 6 : 0;
            break;
        case '=':
            statePC = statePC == 6 ? 7 : 0;
            break;
        case '(':
            statePC = statePC == 7 ? 8 : 0;
            break;
        case ')':
            if(statePC == 8){
                unBridgePCFromLine();
                return;
            }
            break;
        default:
            statePC = 0;
        }
        UARTPutchElev(UART6_BASE, ch);
    }
    while(UARTGetchElev_NB(UART6_BASE, &ch)) {
        switch(ch){
        case '\'':
            stateLine = stateLine == 0 ? 1 : 0;
            break;
        case '\"':
            stateLine = stateLine == 1 ? 2 : 0;
            break;
        case '+':
            stateLine = stateLine == 2 ? 3 : 0;
            break;
        case '!':
            stateLine = stateLine == 3 ? 4 : 0;
            break;
        case '%':
            stateLine = stateLine == 4 ? 5 : 0;
            break;
        case '/':
            stateLine = stateLine == 5 ? 6 : 0;
            break;
        case '=':
            stateLine = stateLine == 6 ? 7 : 0;
            break;
        case '(':
            stateLine = stateLine == 7 ? 8 : 0;
            break;
        case ')':
            if(stateLine == 8){
                unBridgePCFromLine();
                return;
            }
            break;
        default:
            stateLine = 0;
            break;
        }
        UARTPutchElev(UART0_BASE, ch);
    }
}

bool bridgeGPS22Line(void)
{
    if(bridged)
        return 0;
    bridged = true;
    UARTTransferLineDataEnable = false;
    UARTDisableBase(UART6_BASE);
    UARTClearBuffers(UART6_BASE);
    UARTDisableBase(UART3_BASE);
    UARTClearBuffers(UART3_BASE);
    bridgeGPS2LineEnable = true;
    UARTPrint(UART0_BASE, "\r\n GPS2 and line is now bridged.\r\n");
    return 1;
}

void unBridgeGPS2FromLine(void)
{
    if(!bridgeGPS2LineEnable)
        return;
    bridgeGPS2LineEnable = false;
    UARTClearBuffers(UART3_BASE);
    UARTEnableBase(UART3_BASE);
    UARTClearBuffers(UART6_BASE);
    UARTEnableBase(UART6_BASE);
    UARTTransferLineDataEnable = true;
    bridged = false;
    UARTPrint(UART0_BASE, "\r\n GPS2 and line is now UNbridged.\r\n");
}


void bridgeGPS22Line_bridge(void){
    static int stateLine = 0;
    uint8_t ch;

    while(UARTGetchElev_NB(UART3_BASE, &ch)) {
        UARTPutchElev(UART6_BASE, ch);
    }
    while(UARTGetchElev_NB(UART6_BASE, &ch)) {
        switch(ch){
        case '\'':
            stateLine = stateLine == 0 ? 1 : 0;
            break;
        case '\"':
            stateLine = stateLine == 1 ? 2 : 0;
            break;
        case '+':
            stateLine = stateLine == 2 ? 3 : 0;
            break;
        case '!':
            stateLine = stateLine == 3 ? 4 : 0;
            break;
        case '%':
            stateLine = stateLine == 4 ? 5 : 0;
            break;
        case '/':
            stateLine = stateLine == 5 ? 6 : 0;
            break;
        case '=':
            stateLine = stateLine == 6 ? 7 : 0;
            break;
        case '(':
            stateLine = stateLine == 7 ? 8 : 0;
            break;
        case ')':
            if(stateLine == 8){
                unBridgeGPS2FromLine();
                return;
            }
            break;
        default:
            stateLine = 0;
            break;
        }
        UARTPutchElev(UART3_BASE, ch);
    }
}
