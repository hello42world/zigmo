#ifndef ZIGMO_BUTTON_H
#define ZIGMO_BUTTON_H

#include "hal_types.h"
#include "OSAL.h"

#define ZIGMO_BTN_CHANGE 0xD0

#define ZIGMO_BTN_OSAL_EVT      0x0080
#define ZIGMO_BTN_OSAL_EVT_MASK 0x00F0

#define ZIGMO_BTN_0 0
#define ZIGMO_BTN_1 1
#define ZIGMO_BTN_2 2
#define ZIGMO_BTN_3 3


//#define ZIGMO_BUTTON_EVT   0x2000
//#define ZIGMO_BUTTON_MSG      0xD0

#define ZIGMO_BTN_EVT_LONG_PRESS  1
//

// Typedefs
typedef struct ZigmoButton ZigmoButton;


typedef struct  {
  osal_event_hdr_t hdr;
  uint8 btn_id;
//  uint8 evt_type;
} ZigmoButtonEvt;

void zigmo_buttons_init(uint8 task_id);
//ZigmoButton* zigmo_button_create(uint8 task_id, uint8 btn_id);
void zigmo_button_process_osal_evt(uint16 events);
void zigmo_button_notify_hw_state(uint8 btn_id, bool pressed);


#endif