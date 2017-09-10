#ifndef __SPI_H__
#define __SPI_H__

// Wiznet W5100, коды для записи и чтения
#define WIZNET_WRITE_OPCODE 0xF0
#define WIZNET_READ_OPCODE 0x0F
// Порты для Mega128 SPI I/O
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PORTB0


void SPI_Write(uint16_t addr,uint8_t data);
unsigned char SPI_Read(uint16_t addr);

void SPI_WriteMem(uint16_t addr,uint8_t data);
unsigned char SPI_ReadMem(uint16_t addr);

#endif
