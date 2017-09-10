#include <avr/io.h>
#include "Globals.h"
#include <stdio.h>
//#include <avr32/time.h>
#include <avr/pgmspace.h>
#include "utf8Cirillic.h"
#include "logger.h"
#include "linesimulater.h"
#include "controller.h"

uint8_t xmlParseState=0;
uint8_t command;// 1- configure, 2-
uint16_t _il_id, _exp_id;


//STEP step;

void parse_il_id(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<il_id>"));
  sscanf_P(p1,PSTR("<il_id>%u</il_id>"),&_il_id);
}

void parse_exp_id(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<exp_id>"));
  sscanf_P(p1,PSTR("<exp_id>%u</exp_id>"),&_exp_id);
}

void parse_start_time(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<start_time>"));
  sscanf_P(p1,PSTR("<start_time>%lu</start_time>"),&controller_start_time);
}
/*
void parse_measurementCount(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<measurementCount>"));
  sscanf_P(p1,PSTR("<measurementCount>%u</measurementCount>"),&measurementCount);
}
*/
void parse_distance(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<wareData distance=\""));
  sscanf_P(p1,PSTR("<wareData distance=\"%u\""),&controller_distance);
}



char* parseNextStepTag(char* p, uint8_t sNum){
  char* p1=strstr_P(p,PSTR("<step"));
  char* p2;
  char* p3;

  STEP *step=&(controller_steps[sNum]);
 //          logger("parseNextStepTag_1:1\n");
  if (p1){
//           logger("parseNextStepTag_1:2\n");
    p2=strstr_P(p1,PSTR("/>"));
	if (p2){
//          logger("parseNextStepTag_1:3\n");
       *p2=0;

       logger(p1);logger("\n");

       step->impedance=0xFFFF;
       step->attenuation=0xFFFF;
       step->resistance=0xFFFF;
       step->breakage=0;

       p3=strstr_P(p1,PSTR("time_offset="));
       sscanf_P(p3,PSTR("time_offset=\"%lu\""),&step->time_offset);

	   if (strstr_P(p1,PSTR("norm"))){
	   }
	   else if (strstr_P(p1,PSTR("line_shorting"))){
          step->impedance=0;
	   }
	   else if (strstr_P(p1,PSTR("earth_shorting"))){
          step->resistance=0;
	   }
	   else if (strstr_P(p1,PSTR("line_break"))){
          step->breakage=1;
	   }
	   else {

	   }
	   *p2='/';
	   return p2+2;
	} else {// Finish step tag not found
//       return 0;
//       return p1;
       return p;
	}
    return 0;
  }
  else if (strstr_P(p,PSTR("</wareData>"))){
     return 0;
  } else 
     return p;
}

uint16_t xmlParse(uint16_t rsize){
   char *pp0, *pp1;
   if (rsize>0){
     switch (xmlParseState){
       case 0:// expect tag action
         if(strstr_P((char *)globalBuf,PSTR("<action>start</action>"))){
           logger("Got start command\n");
           if (controller_state == CONTROLLER_STATE_WAITING_START
		    || controller_state == CONTROLLER_STATE_SUCCESS
			|| controller_state == CONTROLLER_STATE_PROCESS_STOPPED
			|| controller_state == CONTROLLER_STATE_EXP_ERROR
			){
              parse_il_id();
              parse_exp_id();
			  if (_il_id==controller_il_id && _exp_id==controller_exp_id) {
                parse_start_time();
			    controller_start();
  	            sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_ok,"start",_il_id,_exp_id);
			  }
			  else {
	            sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_error,"start",_il_id,_exp_id,controller_state,controller_il_id,controller_exp_id);
			  }
		   } 
		   else  {
	          sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_error,"start",_il_id,_exp_id,controller_state,controller_il_id,controller_exp_id);
		   }
		   return 0;
		 }
		 else if (strstr_P((char *)globalBuf,PSTR("<action>stop</action>"))){
           logger("Got stop command\n");
           if (controller_state == CONTROLLER_STATE_PROCESS_IN_PROGRESS){
              parse_il_id();
              parse_exp_id();
			  if (_il_id==controller_il_id && _exp_id==controller_exp_id) {
			     controller_stop();
		         sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_ok,"stop",_il_id,_exp_id);
			  }
			  else {
	             sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_error,"stop",_il_id,_exp_id,controller_state,controller_il_id,controller_exp_id);
              }
		   }
		   else {
	          sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_error,"stop",_il_id,_exp_id,controller_state,controller_il_id,controller_exp_id);
		   }
		   return 0;
		 }
		 else if (strstr_P((char *)globalBuf,PSTR("<action>getState</action>"))){
           logger("Got getState command\n");
           parse_il_id();
		   sprintf_P ((char *)globalBuf, fromILtoFA_SPS_getState,controller_state,controller_il_id,controller_exp_id,getElapsedTime(),globalTimeSec,controller_currentStep);
		   return 0;
		 }
		 else if (strstr_P((char *)globalBuf,PSTR("<action>configure</action>"))==0){
           sprintf_P ((char *)globalBuf, fromILtoFA_SPS_bad_XML_format);
           logger("Bad XML format\n");
		   return 0;
		 }
//       case 1:// Configure command parsing in progress ...
////	       xmlParseState=1;
           logger("Got configure command\n");
		   if (  controller_state != CONTROLLER_STATE_NOT_CONFIGURED
		      && controller_state != CONTROLLER_STATE_WAITING_START
			  && controller_state != CONTROLLER_STATE_SUCCESS
              && controller_state != CONTROLLER_STATE_PROCESS_STOPPED
			  && controller_state != CONTROLLER_STATE_CONFIG_ERROR
			  && controller_state != CONTROLLER_STATE_EXP_ERROR
		      ){
		      // Not waiting for configuration ...
	          sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_error,"configure",_il_id,_exp_id,controller_state,controller_il_id,controller_exp_id);
		      return 0;
		   }

           parse_il_id();
           parse_exp_id();
//           parse_measurementCount();
           parse_distance();
           controller_stepsNumber=0;
       case 1:// Configure command parsing in progress ...
//	       xmlParseState=1;
           pp0=(char *)globalBuf;
		   pp1=pp0;
//           controller_stepsNumber=0;
           while (pp1=parseNextStepTag(pp0,controller_stepsNumber++)){
//  sprintf_P ((char *)globalBuf1, PSTR("%u-%u\n"),pp0,pp1);
//  logger((char *)globalBuf1);
		      if (pp1 == pp0){
                 xmlParseState=1;
// logger("---------------------\n");
                controller_stepsNumber--;
				 return (uint16_t)(pp0-(char*)globalBuf);
			  }
			  pp0 = pp1;
              logger("Configure step elapsed\n");
              if(controller_stepsNumber>STEPSBUFSIZE) break;
           }
           controller_stepsNumber--;
           controller_il_id=_il_id;
		   controller_exp_id=_exp_id;
           controller_state=CONTROLLER_STATE_WAITING_START;
		   sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_ok,"configure",_il_id,_exp_id);
           xmlParseState=0;
		   return 0;
	     break;
     }
   }
   else {

   }
   return 1;
}
