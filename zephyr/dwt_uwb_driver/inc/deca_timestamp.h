#ifndef DECA_TIMESTAMP_H
#define DECA_TIMESTAMP_H
#include <zephyr/kernel.h>

uint64_t get_tx_timestamp_u64(void);
uint64_t get_rx_timestamp_u64(void);
uint64_t final_msg_get_ts(const uint8_t *ts_field);
void final_msg_set_ts(uint8_t *ts_field, uint64_t ts);

#endif