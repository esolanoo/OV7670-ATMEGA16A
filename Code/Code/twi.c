#include <avr/io.h>
#include <util/twi.h>
#include "twi.h"

void TWI_init(){
	TWSR = (0<<TWPS0)|(0<<TWPS1);
	TWBR = 72;		// Baud rate forced to 100kHz
					// f = 16000000 Hz / (16 + 2*72 * 1) = 100 kHz
}


void TWI_start(uint8_t adrss){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA) ;
	while(!(TWCR&(1<<TWINT))){}
	_delay_ms(5);
	TWDR = adrss;
	TWCR = (1<<TWEN)|(1<<TWINT);
	while(!(TWCR&(1<<TWINT))){}
}


void TWI_stop(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while (TWCR&(1<<TWSTO)){}
	_delay_ms(5);
}


void TWI_write(uint8_t data){
	TWDR = data;				// Move to I2C reg
	TWCR = (1<<TWINT)|(1<<TWEN);	// Enable I2C + Clear interrupt
	while (!(TWCR&(1<<TWINT))){}
}


void write_reg(uint16_t addr, uint8_t reg, uint8_t dat){
	TWI_start(addr);
	TWI_write(reg);
	TWI_write(dat);
	TWI_stop();
	_delay_ms(10);
}

uint8_t TWI_read(uint8_t adrr, uint8_t reg){
	TWI_start(adrr);
	TWDR = reg;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT))){}
	return TWDR;
}