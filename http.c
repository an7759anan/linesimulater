#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "Globals.h"
#include "SPI.h"
#include "W5100.h"
#include "logger.h"
#include "xmlparser.h"
#include "linesimulater.h"

#include "utf8CirillicDefaultResponce.h"


int clo=0;

int sendAnswerOnGet(void){

  uint16_t v,w;

  char* p1=strstr_P((char *)globalBuf,PSTR("line_length="));
  if (p1) {
     sscanf_P(p1,PSTR("line_length=%u&"),&v);
	 setLineLength(v);
  
     p1=strstr_P((char *)globalBuf,PSTR("breakage="));
     if (p1) setbreakage(1);
	 else setbreakage(0);

     p1=strstr_P((char *)globalBuf,PSTR("impendance="));
     sscanf_P(p1,PSTR("impendance=%u&"),&v);
	 setimpendance(v);

     p1=strstr_P((char *)globalBuf,PSTR("attenuation="));
     sscanf_P(p1,PSTR("attenuation=%u&"),&v);
	 setattenuation(v);

     p1=strstr_P((char *)globalBuf,PSTR("resistance="));
     sscanf_P(p1,PSTR("resistance=%u&"),&v);
	 setresistance(v);

  }


  sprintf_P ((char *)globalBuf, defaultResponce0);
  send(0,globalBuf,strlen((char *)globalBuf));
  sprintf_P ((char *)globalBuf, defaultResponce1,getLineLength(),getbreakage()?"checked":"");
  sprintf_P ((char *)globalBuf1, PSTR("%X\r\n"),strlen((char *)globalBuf)-2);
  send(0,globalBuf1,strlen((char *)globalBuf1));
  if (send(0,globalBuf,strlen((char *)globalBuf))==0) return 0; 
  
  v=getimpendance();
  w=getattenuation();
  sprintf_P ((char *)globalBuf, defaultResponce2,v==65535?"selected":"",v==0?"selected":"",v==50?"selected":"",v==100?"selected":"",v==300?"selected":"",w==65535?"selected":"",w==10?"selected":"",w==40?"selected":"");
  sprintf_P ((char *)globalBuf1, PSTR("%X\r\n"),strlen((char *)globalBuf)-2);
  send(0,globalBuf1,strlen((char *)globalBuf1));
  if (send(0,globalBuf,strlen((char *)globalBuf))==0) return 0; 

  v=getresistance();
  sprintf_P ((char *)globalBuf, defaultResponce3,v==65535?"selected":"",v==0?"selected":"",v==100?"selected":"",v==20000?"selected":"");
  sprintf_P ((char *)globalBuf1, PSTR("%X\r\n"),strlen((char *)globalBuf)-2);
  send(0,globalBuf1,strlen((char *)globalBuf1));
  if (send(0,globalBuf,strlen((char *)globalBuf))==0) return 0; 
  sprintf_P ((char *)globalBuf1, PSTR("0\r\n\r\n"));
  send(0,globalBuf1,strlen((char *)globalBuf1));
  return 1;
}

uint16_t port;

int http(void){
  uint16_t rsize;
    switch(getSockState(0)) {
      case SOCK_CLOSED:
		  logger("SOCK_CLOSED\n");
  eeprom_read_block((void*)&port, (const void*)20, 2);
        if (socket(0,MR_TCP,port) > 0) {
	      if (listen(0) <= 0) _delay_ms(1);
	    }
	    break;
      case SOCK_ESTABLISHED:
		  logger("SOCK_ESTABLISHED\n");
	    // Get the client request size
        if (clo==0) _delay_ms(500);// «адержка дл€ того, чтобы данные успели поступить на вход после соединени€
        rsize=recv_size(0); 
        _delay_ms(50);
        // ѕри установлении соединени€ длина прин€того сообщени€ может быть равна 0.
	    // Ёто приводит к зацикливанию (зависанию сервера).
	    // ƒл€ разрыва зацикливани€ установлен счетчик clo.
	    // ≈сли значение clo при нулевой длине сообщени€ >=5, то даетс€ команда
	    // разрыва соединени€
        clo++;
        // «адержка в одну секунду, чтобы подождать передачу данных, а затем сбросить соединение
        // ѕо истечению 1 сек, если данные не успеют отобразитьс€ на браузере, произойдет разрыв
        // соединени€ и браузер сбросит страничку. ¬ этом проблема этого простейшего сервера. 
        if(clo>=5 && rsize==0) {
	      for(int i=0;i<5;i++) _delay_ms(200);
		  disconnect(0);
		  clo=0; 
	    }    
        if (rsize > 0){
          clo=0;
          // Now read the client Request
         if (rsize> MAX_BUF) rsize=MAX_BUF;
         recv(0,globalBuf,rsize);
          // Check the Request Header
		  logger((char*)globalBuf);
		  if (strstr_P((char *)globalBuf,PSTR("GET"))){
  		    logger("GET has been got\n");

	        // Create the HTTP Response	Header
            if(sendAnswerOnGet()==0) break;

  		    logger("GET has been performed\n");
          } else if (strstr_P((char *)globalBuf,PSTR("POST"))) {
              while (1){
		         uint16_t res=xmlParse(rsize);
			     if (res==0) {
	                if (send(0,globalBuf,strlen((char *)globalBuf))==0)
					    logger("answer sending on POST failed\n");
                    else
					    logger("answer sending on POST success\n");
					break; 
			     } 
			     else {
				    uint8_t* p=globalBuf;
					uint16_t rest=rsize-res;
                    for (;res<rsize;res++){
                       *p++=globalBuf[res];
					}
                    rsize=recv_size(0); 
                    if (rsize>0){
                       if (rsize> MAX_BUF-rest+1) rsize=MAX_BUF-rest+1;
                       recv(0,p,rsize);
                       rsize+=rest;
		            }
					else {
					   break;
					}
			     }
			  }
		  }
	      // Disconnect the socket
          disconnect(0); 	
        } else _delay_us(10);    // Wait for request
	    break;
      case SOCK_FIN_WAIT:   
		  logger("SOCK_FIN_WAIT...\n");
      case SOCK_CLOSING:    
      case SOCK_TIME_WAIT:  
      case SOCK_CLOSE_WAIT:
      case SOCK_LAST_ACK:   
        // Force to close the socket
        close(0);
	   break; 
	   default: 
	   break;
    }
  return 0;
}
