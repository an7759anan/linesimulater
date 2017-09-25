#include <avr/io.h>
#include "Globals.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include "utf8Cirillic.h"
#include "logger.h"
#include "linesimulater.h"
#include "controller.h"
#include "NTPclient.h"
#include "W5100.h"

uint8_t xmlParseState=0;
uint8_t command;// 1- configure, 2-
uint16_t _il_id, _exp_id, _maxTime;

uint8_t parse_il_id(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<il_id>"));
  if (p1) {
     if (sscanf_P(p1,PSTR("<il_id>%u</il_id>"),&_il_id)) return 1;
  }
  return 0;
}

uint8_t parse_exp_id(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<exp_id>"));
  if (p1) {
     if (sscanf_P(p1,PSTR("<exp_id>%u</exp_id>"),&_exp_id)) return 1;
  }
  return 0;
}

uint8_t parse_start_time(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<start_time>"));
  if (p1) {
     if (sscanf_P(p1,PSTR("<start_time>%lu</start_time>"),&controller_start_time)) return 1;
  }
  return 0;
}

uint8_t parse_maxTime(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<maxTime>"));
  if (p1) {
     if (sscanf_P(p1,PSTR("<maxTime>%u</maxTime>"),&_maxTime)) return 1;
  }
  return 0;
}

uint8_t parse_distance(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<wareData distance=\""));
  if (p1) {
     if (sscanf_P(p1,PSTR("<wareData distance=\"%u\""),&controller_distance)) return 1;
  }
  return 0;
}

uint8_t ip[4];
uint8_t netmask[4];
uint8_t loggerip[4];
uint8_t ntp[4];

uint16_t ipport;
uint16_t loggerport;

uint16_t v;

void parse_ip(){
  char *p1=strstr_P((char *)globalBuf,PSTR("<ip>"));
  sscanf_P(p1,PSTR("<ip>%hu."),&v); ip[0]=v;
  p1=strstr_P(p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); ip[1]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); ip[2]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu</ip>"),&v); ip[3]=v;
}

void parse_ipport(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<ipport>"));
  sscanf_P(p1,PSTR("<ipport>%u</ipport>"),&ipport);
}

void parse_netmask(){
  char *p1=strstr_P((char *)globalBuf,PSTR("<netmask>"));
  sscanf_P(p1,PSTR("<netmask>%hu."),&v); netmask[0]=v;
  p1=strstr_P(p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); netmask[1]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); netmask[2]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu</netmask>"),&v); netmask[3]=v;
}

void parse_loggerip(){
  char *p1=strstr_P((char *)globalBuf,PSTR("<logger>"));
  sscanf_P(p1,PSTR("<logger>%hu."),&v); loggerip[0]=v;
  p1=strstr_P(p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); loggerip[1]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); loggerip[2]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu</logger>"),&v); loggerip[3]=v;
}

void parse_loggerport(){
  char* p1=strstr_P((char *)globalBuf,PSTR("<loggerport>"));
  sscanf_P(p1,PSTR("<loggerport>%u</loggerport>"),&loggerport);
}

void parse_ntp(){
  char *p1=strstr_P((char *)globalBuf,PSTR("<ntp>"));
  sscanf_P(p1,PSTR("<logger>%hu."),&v); ntp[0]=v;
  p1=strstr_P(p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); ntp[1]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu."),&v); ntp[2]=v;
  p1=strstr_P(++p1,PSTR("."));
  sscanf_P(p1,PSTR(".%hu</ntp>"),&v); ntp[3]=v;
}


char* parseNextStepTag(char* p, uint8_t sNum){
  char* p1=strstr_P(p,PSTR("<step"));
  char* p2;
  char* p3;

  STEP *step=&(controller_steps[sNum]);
  if (p1){
    p2=strstr_P(p1,PSTR("/>"));
	if (p2){
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
	   else if (strstr_P(p1,PSTR("crushed"))){
          step->breakage=1;
	   }
	   else if (strstr_P(p1,PSTR("bended"))){
          step->breakage=1;
	   }
	   else if (strstr_P(p1,PSTR("overheated"))){
          step->breakage=1;
	   }
	   else if (strstr_P(p1,PSTR("inside explosion funnel"))){
          step->breakage=1;
	   }
	   else if (strstr_P(p1,PSTR("fail command post"))){
          step->breakage=1;
	   }
	   else if (strstr_P(p1,PSTR("overtension"))){
          step->breakage=1;
	   }
	   *p2='/';
	   return p2+2;
	} else {// Finish step tag not found
       return p;
	}
    return 0;
  }
  else if (strstr_P(p,PSTR("</wareData>"))){
     return 0;
  } else 
     return p;
}

uint16_t countChar(char *buf, char c){
   uint16_t count = 0;
   char *p = buf;
   while (*p)
      if (*p++ == c) count++;
   return count;
}

uint8_t checkStartXML(){
   char *root0=strstr_P((char *)globalBuf,PSTR("<root>"));
   char *root1=strstr_P((char *)globalBuf,PSTR("</root>"));
   char *action0=strstr_P((char *)globalBuf,PSTR("<action>"));
   char *action1=strstr_P((char *)globalBuf,PSTR("</action>"));
   char *parameters0=strstr_P((char *)globalBuf,PSTR("<parameters>"));
   char *parameters1=strstr_P((char *)globalBuf,PSTR("</parameters>"));
   char *il_id0=strstr_P((char *)globalBuf,PSTR("<il_id>"));
   char *il_id1=strstr_P((char *)globalBuf,PSTR("</il_id>"));
   char *exp_id0=strstr_P((char *)globalBuf,PSTR("<exp_id>"));
   char *exp_id1=strstr_P((char *)globalBuf,PSTR("</exp_id>"));
   char *start_time0=strstr_P((char *)globalBuf,PSTR("<start_time>"));
   char *start_time1=strstr_P((char *)globalBuf,PSTR("</start_time>"));

   if (root0==0
    || root1==0
    || action0==0
    || action1==0
    || parameters0==0
    || parameters1==0
    || il_id0==0
    || il_id1==0
    || exp_id0==0
    || exp_id1==0
    || start_time0==0
    || start_time1==0
   ) return 0;
/*
   if (strstr_P(root0+1,PSTR("<root>"))) return 0;
   if (strstr_P(root1+1,PSTR("</root>"))) return 0;
   if (strstr_P(action0+1,PSTR("<action>"))) return 0;
   if (strstr_P(action1+1,PSTR("</action>"))) return 0;
   if (strstr_P(parameters0+1,PSTR("<parameters>"))) return 0;
   if (strstr_P(parameters1+1,PSTR("</parameters>"))) return 0;
   if (strstr_P(il_id0+1,PSTR("<il_id>"))) return 0;
   if (strstr_P(il_id1+1,PSTR("</il_id>"))) return 0;
   if (strstr_P(exp_id0+1,PSTR("<exp_id>"))) return 0;
   if (strstr_P(exp_id1+1,PSTR("</exp_id>"))) return 0;
   if (strstr_P(start_time0+1,PSTR("<start_time>"))) return 0;
   if (strstr_P(start_time1+1,PSTR("</start_time>"))) return 0;
*/
   if (! (
       action0 > root0
	&& parameters0 > action1
	&& il_id0 > parameters0
	&& exp_id0 > il_id1
	&& start_time0 > exp_id1
	&& parameters1 > start_time1 
    && root1 > parameters1
   )) return 0;

   if ( !(countChar(root0, '<') == 12 && countChar(root0, '>') == 12)) return 0;
 
   return 1;
}

uint8_t checkStopXML(){
   char *root0=strstr_P((char *)globalBuf,PSTR("<root>"));
   char *root1=strstr_P((char *)globalBuf,PSTR("</root>"));
   char *action0=strstr_P((char *)globalBuf,PSTR("<action>"));
   char *action1=strstr_P((char *)globalBuf,PSTR("</action>"));
   char *parameters0=strstr_P((char *)globalBuf,PSTR("<parameters>"));
   char *parameters1=strstr_P((char *)globalBuf,PSTR("</parameters>"));
   char *il_id0=strstr_P((char *)globalBuf,PSTR("<il_id>"));
   char *il_id1=strstr_P((char *)globalBuf,PSTR("</il_id>"));
   char *exp_id0=strstr_P((char *)globalBuf,PSTR("<exp_id>"));
   char *exp_id1=strstr_P((char *)globalBuf,PSTR("</exp_id>"));

   if (root0==0
    || root1==0
    || action0==0
    || action1==0
    || parameters0==0
    || parameters1==0
    || il_id0==0
    || il_id1==0
    || exp_id0==0
    || exp_id1==0
   ) return 0;

   if (! (
       action0 > root0
	&& parameters0 > action1
	&& il_id0 > parameters0
	&& exp_id0 > il_id1
	&& parameters1 > exp_id1 
    && root1 > parameters1
   )) return 0;

   if ( !(countChar(root0, '<') == 10 && countChar(root0, '>') == 10)) return 0;
 
   return 1;
}

uint8_t checkSettingsXML(){
   char *root0=strstr_P((char *)globalBuf,PSTR("<root>"));
   char *root1=strstr_P((char *)globalBuf,PSTR("</root>"));
   char *action0=strstr_P((char *)globalBuf,PSTR("<action>"));
   char *action1=strstr_P((char *)globalBuf,PSTR("</action>"));
   char *parameters0=strstr_P((char *)globalBuf,PSTR("<parameters>"));
   char *parameters1=strstr_P((char *)globalBuf,PSTR("</parameters>"));
   char *ip0=strstr_P((char *)globalBuf,PSTR("<ip>"));
   char *ip1=strstr_P((char *)globalBuf,PSTR("</ip>"));
   char *ipport0=strstr_P((char *)globalBuf,PSTR("<ipport>"));
   char *ipport1=strstr_P((char *)globalBuf,PSTR("</ipport>"));
   char *netmask0=strstr_P((char *)globalBuf,PSTR("<netmask>"));
   char *netmask1=strstr_P((char *)globalBuf,PSTR("</netmask>"));
   char *logger0=strstr_P((char *)globalBuf,PSTR("<logger>"));
   char *logger1=strstr_P((char *)globalBuf,PSTR("</logger>"));
   char *loggerport0=strstr_P((char *)globalBuf,PSTR("<loggerport>"));
   char *loggerport1=strstr_P((char *)globalBuf,PSTR("</loggerport>"));
   char *ntp0=strstr_P((char *)globalBuf,PSTR("<ntp>"));
   char *ntp1=strstr_P((char *)globalBuf,PSTR("</ntp>"));

   if (root0==0
    || root1==0
    || action0==0
    || action1==0
    || parameters0==0
    || parameters1==0
    || ip0==0
    || ip1==0
    || ipport0==0
    || ipport1==0
	|| netmask0==0
	|| netmask1==0
	|| logger0==0
	|| logger1==0
	|| loggerport0==0
	|| loggerport1==0
	|| ntp0==0
	|| ntp1==0
   ) return 0;

   if (! (
       action0 > root0
	&& parameters0 > action1
	&& ip0 > parameters0
	&& ipport0 > ip1
	&& netmask0 > ipport1 
    && logger0 > netmask1
	&& loggerport0 > logger1
	&& ntp0 > loggerport1
	&& parameters1 > ntp1
   )) return 0;

   if ( !(countChar(root0, '<') == 18 && countChar(root0, '>') == 18)) return 0;
 
   return 1;
}

uint16_t xmlParse(uint16_t rsize){
   char *pp0, *pp1;
   if (rsize>0){
     switch (xmlParseState){
       case 0:// expect tag action
         if(strstr_P((char *)globalBuf,PSTR("<action>start</action>"))){
           logger("Got start command\n");
		   if (checkStartXML()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_bad_XML_format);
              return 0;
		   }
           if (parse_il_id()==0 || parse_exp_id()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_start_stop_absent_params,"start");
              return 0;
		   }
           if (controller_state == CONTROLLER_STATE_WAITING_START
		    || controller_state == CONTROLLER_STATE_SUCCESS
			|| controller_state == CONTROLLER_STATE_PROCESS_STOPPED
			|| controller_state == CONTROLLER_STATE_EXP_ERROR
			){
			  if (_il_id==controller_il_id && _exp_id==controller_exp_id) {
                parse_start_time();
				if (controller_start_time > globalTimeSec) {
			       controller_start();
  	               sprintf_P ((char *)globalBuf, fromILtoFA_SPS_configure_start_stop_ok,"start",_il_id,_exp_id);
				}
				else {
  	               sprintf_P ((char *)globalBuf, fromILtoFA_SPS_start_bad_start_time);
				}
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
		   if (checkStopXML()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_bad_XML_format);
              return 0;
		   }
           if (parse_il_id()==0 || parse_exp_id()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_start_stop_absent_params,"stop");
              return 0;
		   }
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
		   if (checkStopXML()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_bad_XML_format);
              return 0;
		   }
           if (parse_il_id()==0 || parse_exp_id()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_start_stop_absent_params,"getState");
              return 0;
		   }
		   sprintf_P ((char *)globalBuf, fromILtoFA_SPS_getState,controller_state,controller_il_id,controller_exp_id,getElapsedTime(),globalTimeSec,controller_currentStep);
		   return 0;
		 }
		 else if (strstr_P((char *)globalBuf,PSTR("<action>settings</action>"))){
           logger("Got settings command\n");
		   if (checkSettingsXML()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_bad_XML_format);
              return 0;
		   }
           parse_ip();
           parse_ipport();
           parse_netmask();
           parse_loggerip();
           parse_loggerport();
           parse_ntp();
		   sprintf_P ((char *)globalBuf, fromILtoFA_SPS_settings,ip[0],ip[1],ip[2],ip[3],ipport,netmask[0],netmask[1],netmask[2],netmask[3],loggerip[0],loggerip[1],loggerip[2],loggerip[3],loggerport,ntp[0],ntp[1],ntp[2],ntp[3]);
           W5100_SaveSettings(ip, ipport, netmask);
		   logger_SaveSettings(loggerip, loggerport);
		   NTPclient_SaveSettings(ntp);
           needRestart=1;
		   return 0;
		 }
		 else if (strstr_P((char *)globalBuf,PSTR("<action>configure</action>"))==0){
           sprintf_P ((char *)globalBuf, fromILtoFA_SPS_uncknown_command);
           logger("Bad XML format\n");
		   return 0;
		 }
           logger("Got configure command\n");
           if (parse_il_id()==0 || parse_exp_id()==0 || parse_maxTime()==0 || parse_distance()==0){
              sprintf_P ((char *)globalBuf, fromILtoFA_SPS_start_stop_absent_params,"configure");
              return 0;
		   }
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
           controller_stepsNumber=0;
       case 1:// Configure command parsing in progress ...
           pp0=(char *)globalBuf;
		   pp1=pp0;
           while (pp1=parseNextStepTag(pp0,controller_stepsNumber++)){
		      if (pp1 == pp0){
                 xmlParseState=1;
                 controller_stepsNumber--;
				 return (uint16_t)(pp0-(char*)globalBuf);
			  }
			  pp0 = pp1;
              logger("Configure step elapsed\n");
              if(controller_stepsNumber>STEPSBUFSIZE-1) break;
           }
           controller_stepsNumber--;
           controller_il_id=_il_id;
		   controller_exp_id=_exp_id;
           controller_steps[controller_stepsNumber].time_offset=_maxTime*1000;
           controller_steps[controller_stepsNumber].impedance=0xFFFF;
           controller_steps[controller_stepsNumber].attenuation=0xFFFF;
           controller_steps[controller_stepsNumber].resistance=0xFFFF;
           controller_steps[controller_stepsNumber].breakage=0;
	       controller_stepsNumber++;
           if (NTPclient_getState() == NTPCLIENT_STATE_SUSPENSE){
              sendNTPRequest();
              controller_state = CONTROLLER_STATE_IN_CONFIGURING;
           } 
           setLineLength(controller_distance*1000);
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
