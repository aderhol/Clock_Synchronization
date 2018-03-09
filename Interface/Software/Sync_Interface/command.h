#ifndef COMMAND
#define COMMAND

#include <stdint.h>
#include <stdbool.h>

void CommandInterpreter(void);
void execute(uint8_t*, uint32_t);
void setUp(bool);

#endif
