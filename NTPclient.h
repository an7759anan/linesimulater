#ifndef __NTPCLIENT_H__
#define __NTPCLIENT_H__

#define NTPCLIENT_STATE_SUSPENSE 0
#define NTPCLIENT_STATE_SENDREQUEST 1
#define NTPCLIENT_STATE_WAITANSWER 2
#define NTPCLIENT_STATE_FAILED 3

void initNTPclient();
void NTPclient_loop();
void sendNTPRequest();
uint8_t NTPclient_getState();
void NTPclient_SaveSettings(uint8_t *ntp);

#endif
