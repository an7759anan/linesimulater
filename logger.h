#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdint.h>

void initLoggerFromEEPROM();
int logger(const char *buf);
void logger_SaveSettings(uint8_t *loggerip, uint16_t loggerport);

#endif
