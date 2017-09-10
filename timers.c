#include <avr/interrupt.h>
#include <avr/io.h>
#include "Globals.h"

void initTimer(){
    /* Timer clock = I/O clock / 8, OCR1A CTC mode */
    TCCR1B = (1<<CS01)|(1<<WGM12);
	/* 0x2400 for comparisn */
	OCR1A = 0x2400;
    /* Clear Output Compare A Match Flag */
    TIFR  = 1<<OCF1A;
    /* Output Compare A Match Interrupt Enable */
   TIMSK = 1<<OCIE1A;

	/* Enable interrupts */
	SREG = 1<<SREG_I;
}

ISR (TIMER1_COMPA_vect)
{
    /* Toggle a pin on timer overflow */
	if (++_globalTime10ms>99){
	  globalTimeSec++;
      _globalTime10ms=0;
	}
    globalTime10ms++;
}
