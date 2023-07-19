#include <avr/io.h>

void TWI_init();
void TWI_start(uint8_t adrss);
void TWI_stop();
void TWI_write(uint8_t data);
uint8_t TWI_read(uint8_t adrr, uint8_t reg);
void write_reg(uint16_t addr, uint8_t reg, uint8_t dat);		// For OV7670 only