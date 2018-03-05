#ifndef UART_CONTROLLER
#define UART_CONTROLLER

#include <stdbool.h>

void UARTHandler(void);
bool bridgePC2Line(void);
void unBridgePCFromLine(void);
bool bridgeGPS22Line(void);
void unBridgeGPS2FromLine(void);

#endif
