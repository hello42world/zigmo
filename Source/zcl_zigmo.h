/**************************************************************************************************
  Filename:       zcl_zigmo.h
  Revised:        $Date: 2015-08-19 17:11:00 -0700 (Wed, 19 Aug 2015) $
  Revision:       $Revision: 44460 $


  Description:    This file contains the Zigbee Cluster Library Home
                  Automation Sample Application.


  Copyright 2006-2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

#ifndef ZCL_ZIGMO_H
#define ZCL_ZIGMO_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "zcl.h"

/*********************************************************************
 * CONSTANTS
 */
#define ZIGMO_ENDPOINT               8
#define ZIGMO_FIRST_SENSOR_ENDPOINT  9
  
#define LIGHT_OFF                       0x00
#define LIGHT_ON                        0x01

// Events for the sample app
#define SAMPLEAPP_END_DEVICE_REJOIN_EVT   0x0001

#define SAMPLEAPP_END_DEVICE_REJOIN_DELAY 10000

#define NUM_SENSORS 12
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * TYPEDEFS
 */
   
typedef struct {
  zclAttrRec_t attrs[4];
  SimpleDescriptionFormat_t simpleDesc;
  endPointDesc_t endpoint;
  
  int16 measuredValue;
} zigmoSensorEndpoint;

/*********************************************************************
 * VARIABLES
 */
extern SimpleDescriptionFormat_t zclZigmo_SimpleDesc;
//extern SimpleDescriptionFormat_t zclZigmo_SimpleDesc2;

extern CONST zclAttrRec_t zclZigmo_Attrs[];
//extern CONST zclAttrRec_t zclZigmo_Attrs2[];

extern uint16 zclZigmo_IdentifyTime;

extern CONST uint8 zclZigmo_NumAttributes;
//extern CONST uint8 zclZigmo_NumAttributes2;


extern zigmoSensorEndpoint zclZigmo_endpoints[NUM_SENSORS];

/*********************************************************************
 * FUNCTIONS
 */

 /*
  * Initialization for the task
  */
extern void zclZigmo_Init( byte task_id );

extern void zclZigmo_InitMoistureSensors( byte task_id );

/*
 *  Event Process for the task
 */
extern UINT16 zclZigmo_event_loop( byte task_id, UINT16 events );

/*
 *  Reset all writable attributes to their default values.
 */
extern void zclZigmo_ResetAttributesToDefaultValues(void); //implemented in zcl_zigmo_data.c

extern void zclZigmo_InitSensorEndpoint(zigmoSensorEndpoint* endpoint, uint8 endpointId);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ZCL_SAMPLEAPP_H */
