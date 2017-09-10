#ifndef __LINESIMULATER_H__
#define __LINESIMULATER_H__

int linesimulaterInit(void);
int linesimulaterLoop(void);

void setLineLength_debugg(uint16_t lineLength);
void setLineLength(uint16_t lineLength);
void setbreakage(uint8_t breakage);
void setimpendance(uint16_t impendance);
void setattenuation(uint16_t attenuation);
void setresistance(uint16_t resistance);

uint16_t getLineLength(void);
uint8_t getbreakage(void);
uint16_t getimpendance(void);
uint16_t getattenuation(void);
uint16_t getresistance(void);

#endif
