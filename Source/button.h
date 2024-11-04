#ifndef ZIGMO_BUTTON_H
#define ZIGMO_BUTTON_H

#include "hal_types.h"
#include "OSAL.h"


// Target task get this through SYS_EVENT
#define ZIGMO_BTN_EVENT 0xD0

// Up to 4 buttons supported (only 1 for now)
#define ZIGMO_BTN_1 0
#define ZIGMO_BTN_2 1
#define ZIGMO_BTN_3 2
#define ZIGMO_BTN_4 3


// Typedefs
typedef struct ZigmoButton ZigmoButton;

typedef struct  {
  osal_event_hdr_t hdr;
  uint8 btn_id;
} ZigmoButtonEvt;

void zigmo_buttons_init(uint8 task_id);
uint16 zigmo_buttons_event_loop(uint8 task_id, uint16 events);
/*
 Set task where button events will be sent.
*/
void zigmo_buttons_set_target_task(uint8 task_id);


#endif