#include <avr/pgmspace.h>
#include <stdio.h>
#include "Globals.h"
#include "controller.h"
#include "linesimulater.h"
#include "logger.h"
#include "NTPclient.h"

uint8_t controller_state;
STEP controller_steps[STEPSBUFSIZE];
uint8_t controller_stepsNumber;
int8_t controller_currentStep;
uint16_t controller_il_id, controller_exp_id;
uint32_t controller_start_time;
uint16_t controller_distance;
uint16_t _currentTimeOffset10ms;
uint8_t _waitingForTimeOffset;

void controller_init(){
   controller_state=CONTROLLER_STATE_NOT_CONFIGURED;
   globalTime10ms=0;
   globalTime10msInProgress=0;
}

void _start(){
  controller_state = CONTROLLER_STATE_PROCESS_IN_PROGRESS;
  globalTime10ms=0;
  globalTime10msInProgress=1;
  controller_currentStep=-1;
  _waitingForTimeOffset=0;
}

void controller_start(){
  controller_state = CONTROLLER_STATE_WAITING_NTP;
}

void controller_stop(){
   globalTime10msInProgress=0;
   controller_state = CONTROLLER_STATE_PROCESS_STOPPED;
}

void controller_loop(){
   switch(controller_state){
      case CONTROLLER_STATE_WAITING_NTP:
	     if (globalTimeSec >= controller_start_time){
            _start();
		 }
	     break;
      case CONTROLLER_STATE_NOT_CONFIGURED:
	     break;
      case CONTROLLER_STATE_IN_CONFIGURING:
	     if (NTPclient_getState() == NTPCLIENT_STATE_SUSPENSE){
            controller_state=CONTROLLER_STATE_WAITING_START;
		 }
		 else if (NTPclient_getState() == NTPCLIENT_STATE_FAILED){
            controller_state=CONTROLLER_STATE_CONFIG_ERROR;
		 }
	     break;
      case CONTROLLER_STATE_WAITING_START:
	     break;
      case CONTROLLER_STATE_PROCESS_IN_PROGRESS:
	     if (_waitingForTimeOffset){
             if (globalTime10ms<_currentTimeOffset10ms) return;
             sprintf_P ((char *)globalBuf1,
			   PSTR("elapsed_time=%lu\ttime_il=%lu\tdistance=%u\tbreakage=%hu\timpedance=%u\tattenuation=%u\tresistance=%u\n"),
//			         controller_steps[controller_currentStep].time_offset,
			         (uint32_t)globalTime10ms*10,
                     globalTimeSec,
					 controller_distance,
					 controller_steps[controller_currentStep].breakage,
					 controller_steps[controller_currentStep].impedance,
					 controller_steps[controller_currentStep].attenuation,
					 controller_steps[controller_currentStep].resistance);
             logger((char *)globalBuf1);
             setbreakage(controller_steps[controller_currentStep].breakage);
             setimpendance(controller_steps[controller_currentStep].impedance);
             setattenuation(controller_steps[controller_currentStep].attenuation);
             setresistance(controller_steps[controller_currentStep].resistance);
             _waitingForTimeOffset=0;
		 }
		 else {
             if (++controller_currentStep<controller_stepsNumber){
	    		 _currentTimeOffset10ms=controller_steps[controller_currentStep].time_offset/10;
                 _waitingForTimeOffset=1;
		     }
		     else {
                controller_state = CONTROLLER_STATE_SUCCESS;
                globalTime10msInProgress=0;
		     }
		 }
	     break;
      case CONTROLLER_STATE_SUCCESS:
	     break;
      case CONTROLLER_STATE_PROCESS_STOPPED:
	     break;
      case CONTROLLER_STATE_CONFIG_ERROR:
	     break;
   }
}

uint16_t getElapsedTime(){
//   return _currentTimeOffset10ms*10;
   return globalTime10ms*10;
}

