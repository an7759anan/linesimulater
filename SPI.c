#include <avr/pgmspace.h>
#include "SPI.h"


void SPI_Write(uint16_t addr,uint8_t data)
{
  // Activate the CS pin
  SPI_PORT &= ~(1<<SPI_CS);
  // Start Wiznet W5100 Write OpCode transmission
  SPDR = WIZNET_WRITE_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   
  // Start Data transmission
  SPDR = data;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS); 
}
unsigned char SPI_Read(uint16_t addr)
{
  // Activate the CS pin
 SPI_PORT &= ~(1<<SPI_CS);
  // Start Wiznet W5100 Read OpCode transmission
  SPDR = WIZNET_READ_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   
  // Send Dummy transmission for reading the data
  SPDR = 0x00;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  
  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS);
  return(SPDR);
}

void SPI_WriteMem(uint16_t addr,uint8_t data)
{
  // Activate the CS pin
  PORTE &= ~(1<<PORTE7);
  // Start Wiznet W5100 Write OpCode transmission
  SPDR = WIZNET_WRITE_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   
  // Start Data transmission
  SPDR = data;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // CS pin is not active
  PORTE |= (1<<PORTE7); 
}
unsigned char SPI_ReadMem(uint16_t addr)
{
  // Activate the CS pin
  PORTE &= ~(1<<PORTE7);
  // Start Wiznet W5100 Read OpCode transmission
  SPDR = WIZNET_READ_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   
  // Send Dummy transmission for reading the data
  SPDR = 0x00;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  
  // CS pin is not active
  PORTE |= (1<<PORTE7);
  return(SPDR);
}
