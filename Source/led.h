#ifndef ZIGMO_LED_H
#define ZIGMO_LED_H

#include "hal_types.h"
#include "ioCC2530.h"

#define ZIGMO_LED_BLINK_DURATION_MS 300
#define ZIGMO_LED_PIN               P1_0 // Also see zigmo_led_init()

void zigmo_led_init(uint8 task_id);
uint16 zigmo_led_event_loop(uint8 task_id, uint16 events);


void zigmo_led_on(void);
void zigmo_led_off(void);
void zigmo_led_blink(uint8 times);

#endif // ZIGMO_LED_H

