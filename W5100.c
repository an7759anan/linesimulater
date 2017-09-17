#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "W5100.h"
#include "SPI.h"
/*
#define NET_MEMALLOC     0x16     

uint16_t rx_max_buf[]={0x1000,0x0800,0x0800,0};
uint16_t rx_mask[]   ={0x0FFF,0x07FF,0x07FF,0};
uint16_t rx_base[]   ={0x6000,0x7000,0x7800,0x8000};

uint16_t tx_max_buf[]={0x1000,0x0800,0x0800,0};
uint16_t tx_mask[]   ={0x0FFF,0x07FF,0x07FF,0};
uint16_t tx_base[]   ={0x4000,0x5000,0x5800,0x6000};
*/
#define NET_MEMALLOC     0x42

uint16_t rx_max_buf[]={0x1000,0x0400,0x0400,0x0800};
uint16_t rx_mask[]   ={0x0FFF,0x03FF,0x03FF,0x07FF};
uint16_t rx_base[]   ={0x6000,0x7000,0x7400,0x7800};

uint16_t tx_max_buf[]={0x1000,0x0400,0x0400,0x0800};
uint16_t tx_mask[]   ={0x0FFF,0x03FF,0x03FF,0x07FF};
uint16_t tx_base[]   ={0x4000,0x5000,0x5400,0x5800};

void W5100_Init(void)
{
  // Ethernet Setup
  // Setting the Wiznet W5100 Mode Register: 0x0000
  SPI_Write(MR,0x80);            // MR = 0b10000000;
  // Setting the Wiznet W5100 Gateway Address (GAR): 0x0001 to 0x0004
  SPI_Write(GAR + 0,eeprom_read_byte((uint8_t*)0x30));
  SPI_Write(GAR + 1,eeprom_read_byte((uint8_t*)0x31));
  SPI_Write(GAR + 2,eeprom_read_byte((uint8_t*)0x32));
  SPI_Write(GAR + 3,eeprom_read_byte((uint8_t*)0x33));
  // Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
  SPI_Write(SAR + 0,eeprom_read_byte((uint8_t*)0));
  SPI_Write(SAR + 1,eeprom_read_byte((uint8_t*)1));
  SPI_Write(SAR + 2,eeprom_read_byte((uint8_t*)2));
  SPI_Write(SAR + 3,eeprom_read_byte((uint8_t*)3));
  SPI_Write(SAR + 4,eeprom_read_byte((uint8_t*)4));
  SPI_Write(SAR + 5,eeprom_read_byte((uint8_t*)5));
  // Setting the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
  SPI_Write(SUBR + 0,eeprom_read_byte((uint8_t*)0x20));
  SPI_Write(SUBR + 1,eeprom_read_byte((uint8_t*)0x21));
  SPI_Write(SUBR + 2,eeprom_read_byte((uint8_t*)0x22));
  SPI_Write(SUBR + 3,eeprom_read_byte((uint8_t*)0x23));
  // Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
  SPI_Write(SIPR + 0,eeprom_read_byte((uint8_t*)0x10));
  SPI_Write(SIPR + 1,eeprom_read_byte((uint8_t*)0x11));
  SPI_Write(SIPR + 2,eeprom_read_byte((uint8_t*)0x12));
  SPI_Write(SIPR + 3,eeprom_read_byte((uint8_t*)0x13));

  // Setting the Wiznet W5100 RX and TX Memory Size (2KB),
  SPI_Write(RMSR,NET_MEMALLOC);
  SPI_Write(TMSR,NET_MEMALLOC);

  _delay_ms(10);
}

void close(uint8_t sock)
{
   if (sock > 3) return;
   uint16_t offset = sock<<8;
   // Send Close Command
   SPI_Write(S0_CR | offset,CR_CLOSE);
   // Waiting until the S0_CR is clear
   while(SPI_Read(S0_CR | offset));
}

void disconnect(uint8_t sock)
{
   if (sock > 3) return;
   // Send Disconnect Command
   uint16_t offset = sock<<8;
   SPI_Write(S0_CR | offset,CR_DISCON);
   // Wait for Disconecting Process
   while(SPI_Read(S0_CR | offset));
}

uint8_t socket(uint8_t sock,uint8_t eth_protocol,uint16_t tcp_port)
{
    uint8_t retval=0;
	unsigned char state;
    if (sock > 3) return retval;
	uint16_t offset = sock<<8;
    // Make sure we close the socket first
    if (SPI_Read(S0_SR | offset) == SOCK_CLOSED) {
      close(sock);
    }
    // Assigned Socket 0 Mode Register
    SPI_Write(S0_MR | offset,eth_protocol);
    // Now open the Socket 0
    SPI_Write(S0_PORT | offset,((tcp_port & 0xFF00) >> 8 ));
    SPI_Write((S0_PORT  | offset) + 1,(tcp_port & 0x00FF));
    SPI_Write(S0_CR | offset,CR_OPEN);                   // Open Socket
    // Wait for Opening Process
    while(SPI_Read(S0_CR | offset));
    // Check for Init Status
	if (eth_protocol==MR_TCP) state=SOCK_INIT;
	else state=SOCK_UDP;
    if (SPI_Read(S0_SR | offset) == state)
      retval=1;
    else
      close(sock);

    return retval;
}

uint8_t listen(uint8_t sock)
{
   uint8_t retval = 0;
   uint16_t offset = sock<<8;
   if (sock > 3) return retval;
   if (SPI_Read(S0_SR | offset) == SOCK_INIT) {
     // Send the LISTEN Command
     SPI_Write(S0_CR | offset,CR_LISTEN);
     // Wait for Listening Process
     while(SPI_Read(S0_CR | offset));
     // Check for Listen Status
     if (SPI_Read(S0_SR | offset) == SOCK_LISTEN)
       retval=1;
     else
       close(sock);
    }
    return retval;
}

uint16_t send(uint8_t sock,const uint8_t *buf,uint16_t buflen)
{
    uint16_t ptr,offaddr,realaddr,txsize,timeout;   
	uint16_t offset = sock<<8;
    if (buflen <= 0 || sock > 3) return 0;
      // Make sure the TX Free Size Register is available
    txsize=SPI_Read(SO_TX_FSR | offset);
    txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read((SO_TX_FSR | offset) + 1));
    timeout=0;
    while (txsize < buflen) {
      _delay_ms(1);
     txsize=SPI_Read(SO_TX_FSR | offset);
     txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read((SO_TX_FSR|offset) + 1));
     // Timeout for approx 1000 ms
     if (timeout++ > 1000) {
       // Disconnect the connection
       disconnect(sock);
       return 0;
     }
   }	
   // Read the Tx Write Pointer
   ptr = SPI_Read(S0_TX_WR|offset);
   offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read((S0_TX_WR|offset) + 1));
    while(buflen) {
      buflen--;
      // Calculate the real W5100 physical Tx Buffer Address
      realaddr = tx_base[sock] + (offaddr & tx_mask[sock]);
      // Copy the application data to the W5100 Tx Buffer
      SPI_Write(realaddr,*buf);
      offaddr++;
      buf++;
    }
    // Increase the S0_TX_WR value, so it point to the next transmit
    SPI_Write(S0_TX_WR|offset,(offaddr & 0xFF00) >> 8 );
    SPI_Write((S0_TX_WR|offset) + 1,(offaddr & 0x00FF));	
    // Now Send the SEND command
    SPI_Write(S0_CR|offset,CR_SEND);
    // Wait for Sending Process
    while(SPI_Read(S0_CR|offset));	
    return 1;
}

uint16_t recv(uint8_t sock,uint8_t *buf,uint16_t buflen)
{
//    uint16_t ptr,offaddr,realaddr, rxbufaddrbase;   	
    uint16_t ptr,offaddr,realaddr;   	
	uint16_t offset = sock<<8;
    if (buflen <= 0 || sock > 3) return 1;   
    // If the request size > MAX_BUF_WN,just truncate it
    if (buflen > rx_max_buf[sock])
      buflen=rx_max_buf[sock] - 2;
    // Read the Rx Read Pointer
    ptr = SPI_Read(S0_RX_RD|offset);
    offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read((S0_RX_RD|offset) + 1));
    while(buflen) {
      buflen--;
      realaddr=rx_base[sock] + (offaddr & rx_mask[sock]);
      *buf = SPI_Read(realaddr);
      offaddr++;
      buf++;
    }
    *buf='\0';        // String terminated character
    // Increase the S0_RX_RD value, so it point to the next receive
    SPI_Write(S0_RX_RD|offset,(offaddr & 0xFF00) >> 8 );
    SPI_Write((S0_RX_RD|offset) + 1,(offaddr & 0x00FF));	

    // Now Send the RECV command
    SPI_Write(S0_CR|offset,CR_RECV);
    _delay_us(5);    // Wait for Receive Process
    return 1;
}

uint16_t recv_size(uint8_t sock)
{
  uint16_t offset = sock<<8;
  return ((SPI_Read(S0_RX_RSR|offset) & 0x00FF) << 8 ) + SPI_Read((S0_RX_RSR|offset) + 1);
}

void setDestination(uint8_t sock, unsigned char *dst_ip_addr,uint16_t dst_port){
   uint16_t offset = sock<<8;
   SPI_Write((S0_DIPR|offset) + 0,dst_ip_addr[0]);
   SPI_Write((S0_DIPR|offset) + 1,dst_ip_addr[1]);
   SPI_Write((S0_DIPR|offset) + 2,dst_ip_addr[2]);
   SPI_Write((S0_DIPR|offset) + 3,dst_ip_addr[3]);
   SPI_Write(S0_DPORT | offset,((dst_port & 0xFF00) >> 8 ));
   SPI_Write((S0_DPORT  | offset) + 1,(dst_port & 0x00FF));
}

void connect(uint8_t sock){
   uint16_t offset = sock<<8;
   SPI_Write(S0_CR | offset,CR_CONNECT);
}

uint8_t getSockState(uint8_t sock){
   uint16_t offset = sock<<8;
   return SPI_Read(S0_SR|offset);
}


