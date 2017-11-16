#include <stdint.h>
#include <stdbool.h>
#include "i2c_io.h"
#include "driverlib/pin_map.h"  //pin names
#include "driverlib/gpio.h" //GPIO
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"    //interrupt defines (eg. INT_UART0)
#include "driverlib/sysctl.h" //System Control
#include "uart_io.h"

#define TEMPERATURE_SENSORE_ADDRESS 0x48

extern uint32_t SYS_CLK_FREQ_ACTUAL;

void i2cInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //enables I2C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2)); //wait for I2C2 to be ready

    //sets pins as I2C pins
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);
    GPIOPinConfigure(GPIO_PN5_I2C2SCL);

    //configures I2C pins (input/output)
    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);

    I2CMasterInitExpClk(I2C2_BASE, SYS_CLK_FREQ_ACTUAL, false);

    I2CMasterSlaveAddrSet(I2C2_BASE, TEMPERATURE_SENSORE_ADDRESS, false);

    I2CMasterDataPut(I2C2_BASE, 0);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    volatile long double ab = 5311354.35132;
    ab = ab / (-ab / 4.253);
    while(I2CMasterBusy(I2C2_BASE));

}

double temperature(void)
{
    int32_t temp;
    volatile long double ab = 5311354.35132;

    I2CMasterSlaveAddrSet(I2C2_BASE, TEMPERATURE_SENSORE_ADDRESS, true);

    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    ab = ab / (-ab / 4.253);    //delay
    while(I2CMasterBusy(I2C2_BASE));
    temp =  I2CMasterDataGet(I2C2_BASE) << 5;
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    ab = ab / (-ab / 4.253);    //delay
    while(I2CMasterBusy(I2C2_BASE));
    temp += I2CMasterDataGet(I2C2_BASE) >> 3;
    return temp * 0.0625;
}



