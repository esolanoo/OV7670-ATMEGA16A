/*
 * Bluethoot Test
 *
 * Created: 27/abr./21
 * Author : Eduardo Solano Jaime
 *
 * Microcontroller: ATmega16A
 * Frequency: 16MHz (external crystal)
 * Fuse Bits: 99 (high), FF(low)
 * Baud rate: 9600
 *
 * Buetooth module: HC-05
 * Camera module: OV7670
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>				//Microcontroller's general library
#include <stdint.h>				//Use of special variables
#include <util/delay.h>			//Use of delays
#include <avr/interrupt.h>	    //Interruption handling
#include "camera.c"
#include "twi.c"

#define FOSC 16000000			// For USART (same as F_CPU)
#define BAUD 115200				// According to bluetooth module 	
#define MYUBRR FOSC/16/BAUD-1

//"FUNCTION PROTOTYPING"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT); //checks for 0 in specific bit
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);  //checks for 1 in specific bit
void USART_Init(uint16_t ubrr);
void USART_Transmit(uint8_t data);
void send_photo();			// works only with QQVGA
//*************************************************************************

volatile uint8_t serial_data;

void main(void){
	sei();
	USART_Init(MYUBRR);
	camera_init();
	PORTD |= (1<<6);
	_delay_ms(1000);
    while (1){
		if(cero_en_bit(&PIND, 6)){
			_delay_ms(60);
			while(cero_en_bit(&PIND, 6)){}
			_delay_ms(60);
			send_photo();
		}
    }
}


void USART_Init(uint16_t ubrr){
	UBRRH = (ubrr>>8);
	UBRRL = ubrr;
	// Use double speed
	UCSRA |= (1<<U2X);
	// Receiving and transmitting enabling. Receiving interruption
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	// 1 stop bit, 8 bits data, no parity (as requested by bluetooth module)
	UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
}

void USART_Transmit(uint8_t data){
	while(!(UCSRA&(1<<UDRE))){}
	UDR = data;
}

ISR(USART_RXC_vect){
	serial_data = UDR;
}

void send_photo(){ //160x120
	uint8_t d;
	PORTB &= ~(1<<7);
	PORTB |= (1<<5);
	while(cero_en_bit(&PINC, 6)){}			// Wait for high in Vsync
	PORTB |= (1<<7);
	for(uint8_t h=0; h<120; h++){
		for(uint16_t w=0; w<160*2; w++){
			while(cero_en_bit(&PINC, 7)){}		// Wait for high in PCKL
			d = PINA;
			USART_Transmit(d);
			while(uno_en_bit(&PINC, 7)){}		// Wait for low in PCKL
		}
	}
	PORTB &= ~(1<<5);
}

uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT){
	return (!(*LUGAR&(1<<BIT)));
}

uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT){
	return (*LUGAR&(1<<BIT));
}



