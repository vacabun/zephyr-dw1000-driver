#ifndef DW1000_HW_H
#define DW1000_HW_H

int dw1000_hw_init(void);
int dw1000_hw_init_interrupt(void);
void dw1000_hw_fini(void);
void dw1000_hw_reset(void);
void dw1000_hw_wakeup(void);
void dw1000_hw_wakeup_pin_low(void);
void dw1000_hw_interrupt_enable(void);
void dw1000_hw_interrupt_disable(void);

#endif
