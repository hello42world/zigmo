#ifndef ZIGMO_HAL2_H
#define ZIGMO_HAL2_H

#include "ioCC2530.h"



// hal_hey.h
#define ZIGMO_HAL_KEY_BTN_1         0x20
#define ZIGMO_HAL_KEY_STATE_NORMAL  0x00


typedef void (*hal2KeyCBack_t) (uint8 keys, uint8 state);

/**************************************************************************************************
 *                                             FUNCTIONS - API
 **************************************************************************************************/

/*
 * Initialize the Key Service
 */
extern void HalKeyInit( void );

/*
 * Configure the Key Service
 */
extern void HalKeyConfig( bool interruptEnable, const hal2KeyCBack_t cback);

/*
 * Read the Key status
 */
extern uint8 HalKeyRead( void);

/*
 * Enter sleep mode, store important values
 */
extern void HalKeyEnterSleep ( void );

/*
 * Exit sleep mode, retore values
 */
extern uint8 HalKeyExitSleep ( void );

/*
 * This is for internal used by hal_driver
 */
extern void HalKeyPoll ( void );


#endif