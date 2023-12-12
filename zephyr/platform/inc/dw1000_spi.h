#ifndef DW1000_SPI_H
#define DW1000_SPI_H

#include <stdint.h>

int dw1000_spi_init(void);
void dw1000_spi_fini(void);
void dw1000_spi_wakeup(void);
void dw1000_spi_speed_slow(void);
void dw1000_spi_speed_fast(void);
int dw1000_spi_read(uint16_t headerLength, uint8_t* headerBuffer,
					uint16_t readLength, uint8_t* readBuffer);
int dw1000_spi_write(uint16_t headerLength, const uint8_t* headerBuffer,
					 uint16_t bodyLength, const uint8_t* bodyBuffer);
int dw1000_spi_write_crc(uint16_t headerLength, const uint8_t* headerBuffer,
						 uint16_t bodyLength, const uint8_t* bodyBuffer,
						 uint8_t crc8);
#endif
