#ifndef __LINESIMULATER_H__
#define __LINESIMULATER_H__

#include "controller.h"

int linesimulaterInit(void);
int linesimulaterLoop(void);

void setLineLength_debugg(uint16_t lineLength);
void setLineLength(uint16_t lineLength);
void setbreakage(uint8_t breakage);
void setimpendance(uint16_t impendance);
void setattenuation(uint16_t attenuation);
void setresistance(uint16_t resistance);

void setbreakageSwitch(uint8_t breakage, STEP_SWITCH *stepSwitch);
void setimpendanceSwitch(uint16_t impendance, STEP_SWITCH *stepSwitch);
void setattenuationSwitch(uint16_t attenuation, STEP_SWITCH *stepSwitch);
void setresistanceSwitch(uint16_t resistance, STEP_SWITCH *stepSwitch);

uint16_t getLineLength(void);
uint8_t getbreakage(void);
uint16_t getimpendance(void);
uint16_t getattenuation(void);
uint16_t getresistance(void);

#endif
