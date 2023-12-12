#include <zephyr/kernel.h>
#include "deca_device_api.h"
#include "dw1000_hw.h"
#include "deca_port.h"

decaIrqStatus_t decamutexon(void)
{
    dw1000_hw_interrupt_disable();
    return 1;
}

void decamutexoff(decaIrqStatus_t s)
{
    // TODO?: s is not used
    dw1000_hw_interrupt_enable();
}
void deca_sleep(unsigned int time_ms)
{
    k_msleep(time_ms);
}

void deca_usleep(unsigned long time_us)
{
    k_usleep(time_us);
}
int usleep(unsigned long usec)
{
    k_usleep(usec);
}

void Sleep(uint32_t x)
{
    k_msleep(x);
}
