#include "pwm.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h" //System Control
#include "driverlib/gpio.h" //GPIO
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

void PWMInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));

    PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_1);
    GPIOPinConfigure(GPIO_PG1_M0PWM5);
    GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, (PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN | PWM_GEN_MODE_GEN_NO_SYNC | PWM_GEN_MODE_DB_NO_SYNC));
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 1500);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, 750);  //sets duty cycle in PWM clock ticks
    PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void PWMPeriodSet(uint32_t period)
{
    uint32_t pw = PWMPulseWidthGet(PWM0_BASE, PWM_OUT_5);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, period);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, pw);
}

uint32_t PWMPeriodeGet(void)
{
    return PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2);
}

void PWMPulseSet(uint32_t pw)
{
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, pw);
}

uint32_t PWMPulseGet(void)
{
    return PWMPulseWidthGet(PWM0_BASE, PWM_OUT_5);
}
