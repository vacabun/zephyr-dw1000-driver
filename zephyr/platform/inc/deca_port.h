/*! ----------------------------------------------------------------------------
 * @file    port.h
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */


#ifndef PORT_H_
#define PORT_H_

#ifdef __cplusplus
extern "C" {
#endif
decaIrqStatus_t decamutexon(void);
void decamutexoff(decaIrqStatus_t s);
void deca_usleep(unsigned long time_us);
void deca_sleep(unsigned int time_ms);
int usleep(unsigned long usec);
void Sleep(uint32_t x);

#ifdef __cplusplus
}
#endif

#endif /* PORT_H_ */
