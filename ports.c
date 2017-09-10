#include <avr/io.h>
#include "Globals.h"
#include "SPI.h"

void initPorts(){
//	DDRB = 0x2F;	// 0010.1111
	DDRB = 0x2F;	// 0110.0111
	PORTB = 0xFF;	// 1111.1111

//  управление реле
	DDRA = 0xff;	// 1111.1111
	PORTA = 0x00;	// 0000.0000
//	DDRC = 0x30;	// 0011.0000
	DDRC = 0xff;	// 1111.1111
	PORTC = 0x00;	// 0000.0000
  
//	DDRD = 0xE2;	// 1110.0010
//	PORTD = 0x63;	// 0110.0011
	DDRD = 0xC0;	// 1100.0000
	PORTD = 0x03;	// 0000.0011

	DDRE = 0xF0;	// 1111.0000
	PORTE = 0x80;	// 1000.0000

	DDRF = 0x00;	// 0000.0000
	PORTF = 0x00;	// 0000.0000
//for(;;);

	TCNT0 = TIME_ONE_MS;
//TODO	TCCR0A = 0x03;	//0000.0011 - fosc/64
//	TIMSK0|= 0x01; Timer/Counter0 Overflow Interrupt Enable

  SPI_PORT |= (1<<SPI_CS);
  // Enable SPI, Master Mode 0, set the clock rate fck/2
  SPCR = (1<<SPE)|(1<<MSTR);
	SPSR = 1<<SPI2X;
}

