#include <avr/io.h>
#include <util/delay.h>
#include "camera.h"
#include "twi.h"

void camera_init(){
	//PINS and internal configurations
	DDRA = 0;			// For camera data
	DDRC &= ~(1<<7);	// PCKL
	DDRC &= ~(1<<6);	// Vsync 
	DDRC &= ~(1<<5);	// Href 
	DDRB =0xFF;			// (optional) LEDs
	DDRD |= (1<<7);		// For PMW
	TCNT2 = 0;
	OCR2 = 1;			// 0/1/2/128
	TCCR2 = 0b00011001; // Fast PWM, normal op, 1 n (in PB3)
	_delay_ms(10);
	TWI_init();			// I2C 
	_delay_ms(10);
	
	//Basic camera registers to modify
	write_reg(OV7670_W, REG_COM7, 0x80);		// Reset the camera registers to default
	_delay_ms(200);								// Reseting is slow
	write_reg(OV7670_W, REG_TSLB, 0x04); 		// no-negative, normal UV
	write_reg(OV7670_W, REG_COM7, 0);
	
	write_reg(OV7670_W, REG_HSTART, 0x13);		// output format (horizontal) + HREF
	write_reg(OV7670_W, REG_HSTOP, 0x01);		// output format (horizontal) + HREF
	write_reg(OV7670_W, REG_HREF, 0xb6);		// sets horizontal size to 160 pixels
	write_reg(OV7670_W, REG_VSTART, 0x02);		// output format (vertical) + VREF
	write_reg(OV7670_W, REG_VSTOP, 0x7A);		// output format (vertical) + VREF
	write_reg(OV7670_W, REG_VREF, 0x0A);		// sets vertical size to 120 pixels
	write_reg(OV7670_W, REG_COM3, 0);
	write_reg(OV7670_W, REG_COM14, 0);
	
	// Scale 
	write_reg(OV7670_W, 0x70, 0x3A);
	write_reg(OV7670_W, 0x71, 0x35);
	write_reg(OV7670_W, 0x72, 0x11);
	write_reg(OV7670_W, 0x73, 0xF0);
	write_reg(OV7670_W, 0xA2, 0x01);
	write_reg(OV7670_W, REG_COM10, 0);
	
	//Gamma
	write_reg(OV7670_W, 0x7a, 0x20);
	write_reg(OV7670_W, 0x7b, 0x10);
	write_reg(OV7670_W, 0x7c, 0x1e);
	write_reg(OV7670_W, 0x7d, 0x35);
	write_reg(OV7670_W, 0x7e, 0x5a);
	write_reg(OV7670_W, 0x7f, 0x69);
	write_reg(OV7670_W, 0x80, 0x76);
	write_reg(OV7670_W, 0x81, 0x80);
	write_reg(OV7670_W, 0x82, 0x88);
	write_reg(OV7670_W, 0x83, 0x8f);
	write_reg(OV7670_W, 0x84, 0x96);
	write_reg(OV7670_W, 0x85, 0xa3);
	write_reg(OV7670_W, 0x86, 0xaf);
	write_reg(OV7670_W, 0x87, 0xc4);
	write_reg(OV7670_W, 0x88, 0xd7);
	write_reg(OV7670_W, 0x89, 0xe8);
	
	write_reg(OV7670_W, REG_COM8, 0xC0); 		// AGC AWB AEC Unlimited step size
	write_reg(OV7670_W, REG_GAIN, 0); 		
	write_reg(OV7670_W, REG_AECH, 0);
	write_reg(OV7670_W, REG_COM4, 0x40);
	write_reg(OV7670_W, REG_COM9, 0x18);		// 4x gain + COM4 bit
	write_reg(OV7670_W, 0xA5, 0x05);			// 50hz banding	
	write_reg(OV7670_W, 0xAB, 0x07);
	write_reg(OV7670_W, 0x24, 0x95);
	write_reg(OV7670_W, 0x25, 0x33);
	write_reg(OV7670_W, 0x26, 0xE3);
	write_reg(OV7670_W, 0x9F, 0x78);
	write_reg(OV7670_W, 0xA0, 0x68);
	write_reg(OV7670_W, 0xA1, 0x03);
	write_reg(OV7670_W, 0xA6, 0xD8);
	write_reg(OV7670_W, 0xA7, 0xD8);
	write_reg(OV7670_W, 0xA8, 0xF0);
	write_reg(OV7670_W, 0xA9, 0x90);
	write_reg(OV7670_W, 0xAA, 0x94);			// Histogram AEC/AGC controls
	write_reg(OV7670_W, REG_COM8, 0xC5);
	write_reg(OV7670_W, REG_HSYST, 0);
	write_reg(OV7670_W, REG_HSYEN, 0);
	
	//Reserved
	write_reg(OV7670_W, REG_COM5, 0x61);
	write_reg(OV7670_W, REG_COM6, 0x4B);
	write_reg(OV7670_W, 0x16, 0x02);
	write_reg(OV7670_W, REG_MVFP, 0x07);
	write_reg(OV7670_W, 0x21, 0x02);
	write_reg(OV7670_W, 0x22, 0x91);
	write_reg(OV7670_W, 0x29, 0x07);
	write_reg(OV7670_W, 0x33, 0x0B);
	write_reg(OV7670_W, 0x35, 0x0B);
	write_reg(OV7670_W, 0x37, 0x1D);
	write_reg(OV7670_W, 0x38, 0x71);
	write_reg(OV7670_W, 0x39, 0x2A);
	write_reg(OV7670_W, REG_COM12, 0x78);
	write_reg(OV7670_W, 0x4D, 0x40);
	write_reg(OV7670_W, 0x4E, 0x20);
	write_reg(OV7670_W, REG_GFIX, 0);
	write_reg(OV7670_W, 0x74, 0x10);
	write_reg(OV7670_W, 0x8D, 0x4F);
	write_reg(OV7670_W, 0x8E, 0);
	write_reg(OV7670_W, 0x8F, 0);
	write_reg(OV7670_W, 0x90, 0);
	write_reg(OV7670_W, 0x91, 0);
	write_reg(OV7670_W, 0x96, 0);
	write_reg(OV7670_W, 0x9A, 0);
	write_reg(OV7670_W, 0xB0, 0x84);
	write_reg(OV7670_W, 0xB1, 0x0C);
	write_reg(OV7670_W, 0xB2, 0x0E);
	write_reg(OV7670_W, 0xB3, 0x82);
	write_reg(OV7670_W, 0xB8, 0x0A);

	write_reg(OV7670_W, 0x43, 0x0a);
	write_reg(OV7670_W, 0x44, 0xf0);
	write_reg(OV7670_W, 0x45, 0x34);
	write_reg(OV7670_W, 0x46, 0x58);
	write_reg(OV7670_W, 0x47, 0x28);
	write_reg(OV7670_W, 0x48, 0x3a);
	write_reg(OV7670_W, 0x59, 0x88);
	write_reg(OV7670_W, 0x5a, 0x88);
	write_reg(OV7670_W, 0x5b, 0x44);
	write_reg(OV7670_W, 0x5c, 0x67);
	write_reg(OV7670_W, 0x5d, 0x49);
	write_reg(OV7670_W, 0x5e, 0x0e);
	write_reg(OV7670_W, 0x6c, 0x0a);
	write_reg(OV7670_W, 0x6d, 0x55);
	write_reg(OV7670_W, 0x6e, 0x11);
	write_reg(OV7670_W, 0x6f, 0x9e);
	write_reg(OV7670_W, 0x6a, 0x40);
	write_reg(OV7670_W, REG_BLUE, 0x40);
	write_reg(OV7670_W, REG_RED, 0x60);
	write_reg(OV7670_W, REG_COM8, 0xC7);
	
	write_reg(OV7670_W, 0x4F, 0x80);
	write_reg(OV7670_W, 0x50, 0x80);
	write_reg(OV7670_W, 0x51, 0);
	write_reg(OV7670_W, 0x52, 0x22);
	write_reg(OV7670_W, 0x53, 0x5E);
	write_reg(OV7670_W, 0x54, 0x80);
	write_reg(OV7670_W, 0x58, 0x9E);

	write_reg(OV7670_W, REG_COM16, 0x08);
	write_reg(OV7670_W, REG_EDGE, 0);
	write_reg(OV7670_W, 0x75, 0x05);
	write_reg(OV7670_W, 0x76, 0xE1);
	write_reg(OV7670_W, 0x4C, 0);
	write_reg(OV7670_W, 0x77, 0x01);
	write_reg(OV7670_W, REG_COM13, 0x48);
	write_reg(OV7670_W, 0x4B, 0x09);
	write_reg(OV7670_W, 0xC9, 0x60);
	write_reg(OV7670_W, REG_CONTRAS, 0x40);
	write_reg(OV7670_W, 0x34, 0x11);
	write_reg(OV7670_W, REG_COM11, 0x12);
	write_reg(OV7670_W, 0xA4, 0x82);
	write_reg(OV7670_W, 0x96, 0);
	write_reg(OV7670_W, 0x97, 0x30);
	write_reg(OV7670_W, 0x98, 0x20);
	write_reg(OV7670_W, 0x99, 0x30);
	write_reg(OV7670_W, 0x9A, 0x84);
	write_reg(OV7670_W, 0x9B, 0x29);
	write_reg(OV7670_W, 0x9C, 0x03);
	write_reg(OV7670_W, 0x9D, 0x4C);
	write_reg(OV7670_W, 0x9E, 0x3F);
	write_reg(OV7670_W, 0x78, 0x04);
	
	write_reg(OV7670_W, 0x79, 0x01);
	write_reg(OV7670_W, 0xC8, 0xF0);
	write_reg(OV7670_W, 0x79, 0x0F);
	write_reg(OV7670_W, 0xC8, 0);
	write_reg(OV7670_W, 0x79, 0x10);
	write_reg(OV7670_W, 0xC8, 0x7E);
	write_reg(OV7670_W, 0x79, 0x0A);
	write_reg(OV7670_W, 0xC8, 0x80);
	write_reg(OV7670_W, 0x79, 0x0B);
	write_reg(OV7670_W, 0xC8, 0x01);
	write_reg(OV7670_W, 0x79, 0x0C);
	write_reg(OV7670_W, 0xC8, 0x0F);
	write_reg(OV7670_W, 0x79, 0x0D);
	write_reg(OV7670_W, 0xC8, 0x20);
	write_reg(OV7670_W, 0x79, 0x09);
	write_reg(OV7670_W, 0xC8, 0x80);
	write_reg(OV7670_W, 0x79, 0x02);
	write_reg(OV7670_W, 0xC8, 0xC0);
	write_reg(OV7670_W, 0x79, 0x03);
	write_reg(OV7670_W, 0xC8, 0x40);
	write_reg(OV7670_W, 0x79, 0x05);
	write_reg(OV7670_W, 0xC8, 0x30);
	write_reg(OV7670_W, 0x79, 0x26);
	
	//QQVGA
	write_reg(OV7670_W, REG_COM3, 0x04);
	write_reg(OV7670_W, REG_COM14, 0x1A);		// PCLK/4 + manual adjust
	write_reg(OV7670_W, 0x72, 0x22);			// DWC control
	write_reg(OV7670_W, 0x73, 0xF2);			// Division/4 of DSP
	write_reg(OV7670_W, REG_HSTART, 0x16);		// output format (horizontal) + HREF
	write_reg(OV7670_W, REG_HSTOP, 0x04);		// output format (horizontal) + HREF
	write_reg(OV7670_W, REG_HREF, 0xa4);		// sets horizontal size to 160 pixels
	write_reg(OV7670_W, REG_VSTART, 0x02);		// output format (vertical) + VREF
	write_reg(OV7670_W, REG_VSTOP, 0x7A);		// output format (vertical) + VREF
	write_reg(OV7670_W, REG_VREF, 0x0A);		// sets vertical size to 120 pixels
	
	//Select RGB565 + Matrix tweak
	write_reg(OV7670_W, REG_COM7, 0x04); 		// RGB mode
	write_reg(OV7670_W, 0x8C, 0);
	write_reg(OV7670_W, REG_COM1, 0);
	write_reg(OV7670_W, REG_COM15, 0xD0); 		// Full range output, RGB 565
	write_reg(OV7670_W, REG_COM9, 0x6A);
	write_reg(OV7670_W, MTX1, 0xB3);
	write_reg(OV7670_W, MTX2, 0xB3);
	write_reg(OV7670_W, MTX3, 0);
	write_reg(OV7670_W, MTX4, 0x3D);
	write_reg(OV7670_W, MTX5, 0xA7);
	write_reg(OV7670_W, MTX6, 0xE4);			// modify to change color output
	write_reg(OV7670_W, REG_COM13, 0x40);
	
	uint8_t var;
	var = TWI_read(OV7670_R, REG_COM10);
	var |= 0x20;
	write_reg(OV7670_W, REG_COM10, var);		// Disable pclk in blank lines
	var = TWI_read(OV7670_R, REG_COM6);
	var |= 0x80;	
	write_reg(OV7670_W, REG_COM6, var);			// Disable HREF in blank lines
	write_reg(OV7670_W, REG_CLKRC, 0x3F);		// internal clock to max 0x80|prescaler
	var = TWI_read(OV7670_R, DBLV);
	var &= ~(0b11000000);
	write_reg(OV7670_W, DBLV, var|(0<<6));
	
	PORTB |= (1<<7);
}