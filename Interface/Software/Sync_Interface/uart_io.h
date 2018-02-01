#ifndef UART_IO
#define UART_IO

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"

extern bool show_gps, show_line;

void UARTPrint(uint32_t, const uint8_t*);
void UARTPrint_NB(uint32_t, const uint8_t*);
void UARTPrint_i32(uint32_t, int32_t);
void UARTPrint_i32_NB(uint32_t, int32_t);
void UARTInit(void);
void UARTPutch(uint32_t, uint8_t);
void UARTPutch_NB(uint32_t, uint8_t);
bool uartHasData(uint32_t);
uint8_t UARTGetch(uint32_t);

#endif
