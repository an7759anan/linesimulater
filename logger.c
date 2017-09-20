#include <avr/io.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "SPI.h"
#include "W5100.h"
#include "Globals.h"


uint16_t dst_port;

void initLoggerFromEEPROM(){
  uint8_t dst_ip_addr[4];
  eeprom_read_block((void*)dst_ip_addr, (const void*)0x40, 4);
  eeprom_read_block((void*)&dst_port, (const void*)0x44, 2);
  setDestination(1,dst_ip_addr,dst_port);
  socket(1,MR_UDP,dst_port);
}

void logger_SaveSettings(uint8_t *loggerip, uint16_t loggerport){
  eeprom_write_block((void*)loggerip, (const void*)0x40, 4);
  eeprom_write_block((void*)&loggerport, (const void*)0x44, 2);
}


int logger(const char *buf){
  send(1,(uint8_t*)buf,strlen(buf)+1); 
  return 1;
}

