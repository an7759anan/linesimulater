#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "logger.h"
#include "Globals.h"
#include "controller.h"

uint16_t _lineLength;
uint8_t _lineBits;
uint8_t _breakage;
uint16_t _impendance;
uint16_t _attenuation;
uint16_t _resistance;

int linesimulaterInit(void){
   PORTD &= ~(3<<PORTD6);
   PORTA = 0xff;
   PORTC = 0xff;
   PORTD |= 1<<PORTD6;
   _delay_ms(50);
   PORTD &= ~(1<<PORTD6);
   PORTA = 0x00;
   PORTC = 0x00;
   _lineLength=0;
   _lineBits=0;
   _breakage=0;
   _impendance=65535;
   _attenuation=65535;
   _resistance=65535;
   return 0;
}

int linesimulaterLoop(void){

  return 0;
}


void setLineLength_debugg(uint16_t lineLength){
   if (lineLength == _lineLength) return;
   uint8_t bit=0;
   switch(_lineLength){
      case 500:
	     bit = 1<<PORTC0;
	     break;
      case 1000:
	     bit = 1<<PORTC1;
	     break;
      case 1500:
	     bit = 1<<PORTC2;
	     break;
      case 3000:
	     bit = 1<<PORTC3;
	     break;
      case 5000:
	     bit = 1<<PORTC4;
	     break;
      case 10000:
	     bit = 1<<PORTC5;
	     break;
   }
   if (bit){
      PORTC |= bit;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTC &= ~bit;
   } 
   bit=0;
   switch(lineLength){
      case 500:
	     bit = 1<<PORTC0;
	     break;
      case 1000:
	     bit = 1<<PORTC1;
	     break;
      case 1500:
	     bit = 1<<PORTC2;
	     break;
      case 3000:
	     bit = 1<<PORTC3;
	     break;
      case 5000:
	     bit = 1<<PORTC4;
	     break;
      case 10000:
	     bit = 1<<PORTC5;
	     break;
   }
   if (bit){
      PORTC |= bit;
	  PORTD |= 1<<PORTD7;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD7);
      PORTC &= ~bit;
   } else {
      PORTC |= 0x3f;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTC &= ~0x3f;
   }

   _lineLength=lineLength;
};

void setLineLength(uint16_t lineLength){
   //switch off all 
      PORTC |= 0x3F;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTC &= ~0x3F;
   // calculate bit mask for switch on
   uint8_t bit=0;
   uint16_t ll=lineLength;   
   if (ll >= 10000){
      bit |= 0x20; // + 10000
      ll -= 10000;
   }
   if (ll >= 5000){
      bit |= 0x10; // + 5000
	  ll -= 5000;
   }
   if (ll >= 3000){
      bit |= 0x08; // + 3000
	  ll -= 3000;
   }
   if (ll >= 1000){
      bit |= 0x04; // + 1000
	  ll -= 1000;
   }
   if (ll >= 1000){
      bit |= 0x02; // + 1000
	  ll -= 1000;
   }
   if (ll >= 500){
      bit |= 0x01; // + 500
   }
   if (bit){
      PORTC |= bit;
	  PORTD |= 1<<PORTD7;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD7);
      PORTC &= ~bit;
   }
   _lineLength=lineLength;
};

void setbreakage(uint8_t breakage){
   if (breakage == _breakage) return;
   PORTA |= 1<<PORTA7;
   if (breakage==1) PORTD |= 1<<PORTD7;
   else PORTD |= 1<<PORTD6;
   _delay_ms(50);
   PORTD &= ~(3<<PORTD6);
   PORTA &= ~(1<<PORTA7);
   _breakage=breakage;
};

void setbreakageSwitch(uint8_t breakage, STEP_SWITCH *stepSwitch){
   if (breakage == _breakage) return;
   if (breakage==1) stepSwitch->portaSwitchOn |= 1<<PORTA7;
   else stepSwitch->portaSwitchOff |= 1<<PORTA7;
   _breakage=breakage;
}

void setimpendance(uint16_t impendance){
   if (impendance == _impendance) return;
   uint8_t bit=0;
   switch(_impendance){
      case 0:
	     bit = 1<<PORTA6;
	     break;
      case 50:
	     bit = 1<<PORTA5;
	     break;
      case 100:
	     bit = 1<<PORTA4;
	     break;
      case 300:
	     bit = 1<<PORTA3;
	     break;
   }
   if (bit){
      PORTA |= bit;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTA &= ~bit;
   }
   bit=0;
   switch(impendance){
      case 0:
	     bit = 1<<PORTA6;
	     break;
      case 50:
	     bit = 1<<PORTA5;
	     break;
      case 100:
	     bit = 1<<PORTA4;
	     break;
      case 300:
	     bit = 1<<PORTA3;
	     break;
   }
   if (bit){
      PORTA |= bit;
	  PORTD |= 1<<PORTD7;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD7);
      PORTA &= ~bit;
   } else {
      PORTA |= 0x78;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTA &= ~0x78;
   }
   _impendance=impendance;
};

void setimpendanceSwitch(uint16_t impendance, STEP_SWITCH *stepSwitch){
   if (impendance == _impendance) return;
   uint8_t bit=0;
   switch(_impendance){
      case 0:
	     bit = 1<<PORTA6;
	     break;
      case 50:
	     bit = 1<<PORTA5;
	     break;
      case 100:
	     bit = 1<<PORTA4;
	     break;
      case 300:
	     bit = 1<<PORTA3;
	     break;
   }
   if (bit) stepSwitch->portaSwitchOff |= bit;
   bit=0;
   switch(impendance){
      case 0:
	     bit = 1<<PORTA6;
	     break;
      case 50:
	     bit = 1<<PORTA5;
	     break;
      case 100:
	     bit = 1<<PORTA4;
	     break;
      case 300:
	     bit = 1<<PORTA3;
	     break;
   }
   if (bit) stepSwitch->portaSwitchOn |= bit;
   else     stepSwitch->portaSwitchOff |= 0x78;
   _impendance=impendance;
}

void setattenuation(uint16_t attenuation){
   if (attenuation == _attenuation) return;
   uint8_t bit=0;
   switch(_attenuation){
      case 10:
	     bit = 1<<PORTC6;
	     break;
      case 40:
	     bit = 1<<PORTC7;
	     break;
   }
   if (bit){
      PORTC |= bit;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTC &= ~bit;
   }
   bit=0;
   switch(attenuation){
      case 10:
	     bit = 1<<PORTC6;
	     break;
      case 40:
	     bit = 1<<PORTC7;
	     break;
   }
   if (bit){
      PORTC |= bit;
	  PORTD |= 1<<PORTD7;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD7);
      PORTC &= ~bit;
   } else {
      PORTC |= 0xc0;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTC &= ~0xc0;
   }
   _attenuation=attenuation;
};

void setattenuationSwitch(uint16_t attenuation, STEP_SWITCH *stepSwitch){
   if (attenuation == _attenuation) return;
   uint8_t bit=0;
   switch(_attenuation){
      case 10:
	     bit = 1<<PORTC6;
	     break;
      case 40:
	     bit = 1<<PORTC7;
	     break;
   }
   if (bit) stepSwitch->portcSwitchOff |= bit;
   bit=0;
   switch(attenuation){
      case 10:
	     bit = 1<<PORTC6;
	     break;
      case 40:
	     bit = 1<<PORTC7;
	     break;
   }
   if (bit) stepSwitch->portcSwitchOn |= bit;
   else stepSwitch->portcSwitchOff |= 0xc0;
   _attenuation=attenuation;
}

void setresistance(uint16_t resistance){
   if (resistance == _resistance) return;
   uint8_t bit=0;
   switch(_resistance){
      case 0:
	     bit = 1<<PORTA0;
	     break;
      case 100:
	     bit = 1<<PORTA1;
	     break;
      case 20000:
	     bit = 1<<PORTA2;
	     break;
   }
   if (bit){
      PORTA |= bit;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTA &= ~bit;
   }
   bit=0;
   switch(resistance){
      case 0:
	     bit = 1<<PORTA0;
	     break;
      case 100:
	     bit = 1<<PORTA1;
	     break;
      case 20000:
	     bit = 1<<PORTA2;
	     break;
   }
   if (bit){
      PORTA |= bit;
	  PORTD |= 1<<PORTD7;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD7);
      PORTA &= ~bit;
   } else {
      PORTA |= 0x03;
	  PORTD |= 1<<PORTD6;
      _delay_ms(50);
	  PORTD &= ~(1<<PORTD6);
      PORTA &= ~0x03;
   }
   _resistance=resistance;
};

void setresistanceSwitch(uint16_t resistance, STEP_SWITCH *stepSwitch){
   if (resistance == _resistance) return;
   uint8_t bit=0;
   switch(_resistance){
      case 0:
	     bit = 1<<PORTA0;
	     break;
      case 100:
	     bit = 1<<PORTA1;
	     break;
      case 20000:
	     bit = 1<<PORTA2;
	     break;
   }
   if (bit) stepSwitch->portaSwitchOff |= bit;
   bit=0;
   switch(resistance){
      case 0:
	     bit = 1<<PORTA0;
	     break;
      case 100:
	     bit = 1<<PORTA1;
	     break;
      case 20000:
	     bit = 1<<PORTA2;
	     break;
   }
   if (bit) stepSwitch->portaSwitchOn |= bit;
   else    stepSwitch->portaSwitchOff |= 0x03;
   _resistance=resistance;
}

uint16_t getLineLength(void){return _lineLength;};
uint8_t getbreakage(void){return _breakage;};
uint16_t getimpendance(void){return _impendance;};
uint16_t getattenuation(void){return _attenuation;};
uint16_t getresistance(void){return _resistance;};

