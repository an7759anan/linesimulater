#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <util/delay.h>
#include "Globals.h"
#include "W5100.h"
#include "logger.h"
#include "NTPclient.h"

uint8_t packetBuffer[48];
uint8_t NTPclientState;

void initNTPclient(){
//  uint8_t ntp_ip_addr[]={94,100,192,29};
  uint8_t ntp_ip_addr[4];
  eeprom_read_block((void*)ntp_ip_addr, (const void*)0x50, 4);
  setDestination(2,ntp_ip_addr,123);
  socket(2,MR_UDP,8888);

  packetBuffer[0] = 0b11100011;// LI, Version, Mode   
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  NTPclientState=NTPCLIENT_STATE_SUSPENSE;
}

void NTPclient_loop(){
  uint16_t rsize;
//  PORTC = 0x20;
 // sprintf_P ((char*)globalBuf, PSTR("%i"),NTPclientState);
 // logger((char*)globalBuf);
  switch (NTPclientState){
    case NTPCLIENT_STATE_SUSPENSE:
	  break;
    case NTPCLIENT_STATE_SENDREQUEST:
      send(2,packetBuffer,48); 
      _delay_ms(200);
      NTPclientState=NTPCLIENT_STATE_WAITANSWER;
	  break;
    case NTPCLIENT_STATE_WAITANSWER:
        rsize=recv_size(2); 
		if (rsize>0){
          recv(2,globalBuf,rsize);
          globalTimeSec = globalBuf[40];  
          globalTimeSec = (globalTimeSec<<8)|globalBuf[41];  
          globalTimeSec = (globalTimeSec<<8)|globalBuf[42];  
          globalTimeSec = (globalTimeSec<<8)|globalBuf[43];  
          globalTimeSec = globalTimeSec - 2208988800UL;
          globalTime10ms =0;
//          sprintf_P ((char*)globalBuf, PSTR("%lx\r\n"),globalTimeSec);
//          logger((char*)globalBuf);
          NTPclientState=NTPCLIENT_STATE_SUSPENSE;
		}
        NTPclientState=NTPCLIENT_STATE_SUSPENSE;
	  break;
    default:
	  logger("NTP ???\n");
	  break;
  }
}

void sendNTPRequest(){
  if (NTPclientState == NTPCLIENT_STATE_SUSPENSE)
    NTPclientState=NTPCLIENT_STATE_SENDREQUEST;
}

uint8_t NTPclient_getState(){
  return NTPclientState;
}


