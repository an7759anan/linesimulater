#include <avr/interrupt.h>
#include <avr/io.h>
#include "Globals.h"
#include "controller.h"

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


uint8_t switchStep; // 0-before switchOff up, 1-before switchOff down and switchOn up, 2-before switchOn down
STEP_SWITCH *_currentStepSwitch;
uint32_t _currentTimeOffset10ms;
ISR (TIMER1_COMPA_vect)
{
	if (++_globalTime10ms>99){
	  globalTimeSec++;
      _globalTime10ms=0;
	}
//	if (globalTime10msInProgress) globalTime10ms++;
    
	if (controller_state == CONTROLLER_STATE_WAITING_NTP){
	   if (globalTimeSec >= controller_start_time){
          controller_state = CONTROLLER_STATE_PROCESS_IN_PROGRESS;
          globalTime10ms=0;
//          globalTime10msInProgress=1;
          controller_currentStep=0;
//		  _currentTimeOffset10ms=controller_steps_switch[controller_currentStep].time_offset/10;
          _currentStepSwitch=&controller_steps_switch[0];
		  _currentTimeOffset10ms=_currentStepSwitch->time_offset/10;
//          _waitingForTimeOffset=0; 
          switchStep=0;         
	   }
	   else return;
	}
	if (controller_state == CONTROLLER_STATE_PROCESS_IN_PROGRESS){
	   globalTime10ms++;
       switch (switchStep){
          case 0:
		     if (globalTime10ms<_currentTimeOffset10ms) return;
             PORTA = _currentStepSwitch->portaSwitchOff;
             PORTC = _currentStepSwitch->portcSwitchOff;
			 PORTD |= 1<<PORTD6;
             switchStep = 1;
		     break;
          case 1:
		     if (globalTime10ms<_currentTimeOffset10ms+5) return;
	         PORTD &= ~(1<<PORTD6);
             PORTA = _currentStepSwitch->portaSwitchOn;
             PORTC = _currentStepSwitch->portcSwitchOn;
			 PORTD |= 1<<PORTD7;
	         switchStep = 2;
		     break;
          case 2:
		     if (globalTime10ms<_currentTimeOffset10ms+10) return;
	         PORTD &= ~(1<<PORTD7);
	         switchStep = 0;
             if (++controller_currentStep<controller_stepsNumber){
                 _currentStepSwitch=&controller_steps_switch[controller_currentStep];
		         _currentTimeOffset10ms=_currentStepSwitch->time_offset/10;
//                 _waitingForTimeOffset=1;
		     }
		     else {
                controller_state = CONTROLLER_STATE_SUCCESS;
//                logger("experiment was completed successfully\n");
//                globalTime10msInProgress=0;
		     }
			 break;
	   }

	}
}
