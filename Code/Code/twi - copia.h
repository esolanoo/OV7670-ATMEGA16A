#include <avr/io.h>

void TWI_init();
uint8_t TWI_start(uint8_t adrss);
void TWI_stop();
uint8_t TWI_write(uint8_t data);
uint8_t TWI_read();
uint8_t write_reg(uint16_t addr, uint8_t reg, uint8_t dat);		// For OV7670 only