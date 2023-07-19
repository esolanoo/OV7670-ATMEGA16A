#include <avr/io.h>
#include <util/twi.h>
#include "twi.h"

void TWI_init(){
	DDRC &= ~((1<<4)|(1<<5));
	TWBR = 72;				// Baud rate due to fomrula for 100 kHz
	TWSR &= ~0b00000111; 				// Prescaler to 1 (due to formula)
	// SCL freq = 16 000 000 Hz / (16 + 2*72**1) = 100 kHz
}


uint8_t TWI_start(uint8_t adrss){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA) ;
	while(!(TWCR&(1<<TWINT))){}
	if((TWSR&0xF8)!=0x08){ return 0; }
	TWDR = adrss;		
	TWCR = (1<<TWEN)|(1<<TWINT);
	while(!(TWCR&(1<<TWINT)));
	if((TWSR&0xF8)==0x18){ return 1; }	
	if((TWSR&0xF8)==0x20){ return 2; }	
	else { return 3; }
}


void TWI_stop(){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while ((TWCR & (1 << TWSTO)));
}


uint8_t TWI_write(uint8_t data){
	TWDR = data;				// Move to I2C reg
	TWCR = (1<<TWINT)|(1<<TWEN);	// Enable I2C + Clear interrupt
	while (!(TWCR&(1<<TWINT))){}
	if ((TWSR & 0xF8)!=0x28){ return 0; }
	return 1;
}


uint8_t write_reg(uint16_t addr, uint8_t reg, uint8_t dat){
	TWI_start(addr);
	if(TWI_write(reg)){
		if(TWI_write(dat)){
			TWI_stop();
			return 1;
		}
	}
	TWI_stop();
	_delay_ms(10);
	return 0;
}

uint8_t TWI_read(){
	TWCR |= (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT))){}
	return TWDR;
}