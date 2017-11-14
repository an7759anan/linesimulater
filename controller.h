#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <stdint.h>

#define CONTROLLER_STATE_EXP_ERROR 0
#define CONTROLLER_STATE_WAITING_NTP 1
#define CONTROLLER_STATE_NOT_CONFIGURED 4
#define CONTROLLER_STATE_IN_CONFIGURING 6
#define CONTROLLER_STATE_WAITING_START 8
#define CONTROLLER_STATE_PROCESS_IN_PROGRESS 2
#define CONTROLLER_STATE_SUCCESS 3
#define CONTROLLER_STATE_PROCESS_STOPPED 5
#define CONTROLLER_STATE_CONFIG_ERROR 7

#define STEPSBUFSIZE 100

void controller_init();
void controller_start();
void controller_stop();
void controller_loop();
uint16_t getElapsedTime();

extern uint8_t controller_state;

typedef struct _STEP_SWITCH {
  uint32_t time_offset;
  uint8_t portaSwitchOff;
  uint8_t portaSwitchOn;
  uint8_t portcSwitchOff;
  uint8_t portcSwitchOn;
  uint16_t impedance;
  uint16_t attenuation;
  uint16_t resistance;
  uint8_t breakage;
} STEP_SWITCH;

extern STEP_SWITCH controller_steps_switch[];
extern uint8_t controller_stepsNumber;
extern uint16_t controller_il_id;
extern uint32_t controller_exp_id;
extern uint32_t controller_start_time;
extern uint16_t controller_distance;
extern uint8_t controller_currentStep;
extern uint8_t lastConfigureErrorWasNtp;

#endif
