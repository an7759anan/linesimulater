#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Globals.h"
#include "ports.h"
#include "SPI.h"
#include "W5100.h"
#include "http.h"
#include "logger.h"
#include "timers.h"
#include "NTPclient.h"
#include "linesimulater.h"
#include "controller.h"

uint8_t buff[7];

uint32_t time1=0;

int main(void){
  globalBuf[MAX_BUF]=0;// \0 terminator for string functions
  initPorts();
  W5100_Init();
  initLoggerFromEEPROM();
  initNTPclient();
  initTimer();
  linesimulaterInit();
  controller_init();

  for(;;){
     http();
/*
     if (globalTimeSec!=time1){
        time1=globalTimeSec;
 	    sprintf_P ((char *)buff, PSTR("%lu\n"),time1);
        logger("tic...");
	 }
*/
     NTPclient_loop();
     controller_loop();
  }
  return 0;
}
