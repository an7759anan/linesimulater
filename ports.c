#include <avr/io.h>
#include "Globals.h"
#include "SPI.h"

void initPorts(){
	DDRB = 0x2F;	// 0110.0111
	PORTB = 0xFF;	// 1111.1111

//  управление реле
	DDRA = 0xff;	// 1111.1111
	PORTA = 0x00;	// 0000.0000
	DDRC = 0xff;	// 1111.1111
	PORTC = 0x00;	// 0000.0000
  
	DDRD = 0xC0;	// 1100.0000
	PORTD = 0x03;	// 0000.0011

	DDRE = 0xF0;	// 1111.0000
	PORTE = 0x80;	// 1000.0000

	DDRF = 0x00;	// 0000.0000
	PORTF = 0x00;	// 0000.0000


	TCNT0 = TIME_ONE_MS;


  SPI_PORT |= (1<<SPI_CS);
  // Enable SPI, Master Mode 0, set the clock rate fck/2
  SPCR = (1<<SPE)|(1<<MSTR);
	SPSR = 1<<SPI2X;
}

