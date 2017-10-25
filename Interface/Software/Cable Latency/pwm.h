#ifndef PWM
#define PWM

#include <stdint.h>

void PWMInit(void);
void PWMPeriodSet(uint32_t);
uint32_t PWMPeriodeGet(void);
void PWMPulseSet(uint32_t);
uint32_t PWMPulseGet(void);

#endif
