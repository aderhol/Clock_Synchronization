#ifndef SYSTEM_TICK
#define SYSTEM_TICK

#define SYSTEM_TICK_FREQ 10 //[Hz] minimum 7. maximum SYS_CLK_FREQ_ACTUAL

extern uint32_t wdt;

void SysTickInit(void);

#endif
