/**************************************************************************************************
  Filename:       zcl_zigmo_data.c
  Revised:        $Date: 2014-07-30 12:57:37 -0700 (Wed, 30 Jul 2014) $
  Revision:       $Revision: 39591 $


  Description:    Zigbee Cluster Library - sample device application.


  Copyright 2006-2014 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDConfig.h"

#include "zcl.h"
#include "zcl_ms.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_poll_control.h"
#include "zcl_electrical_measurement.h"
#include "zcl_diagnostic.h"
#include "zcl_meter_identification.h"
#include "zcl_appliance_identification.h"
#include "zcl_appliance_events_alerts.h"
#include "zcl_power_profile.h"
#include "zcl_appliance_control.h"
#include "zcl_appliance_statistics.h"
#include "zcl_hvac.h"

#include "zcl_zigmo.h"

/*********************************************************************
 * CONSTANTS
 */

#define ZIGMO_DEVICE_VERSION     1
#define ZIGMO_FLAGS              0

#define ZIGMO_HWVERSION          0
#define ZIGMO_ZCLVERSION         0

#define DEFAULT_PHYSICAL_ENVIRONMENT 0
#define DEFAULT_DEVICE_ENABLE_STATE DEVICE_ENABLED
#define DEFAULT_IDENTIFY_TIME 0

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

//global attributes
const uint16 zclZigmo_clusterRevision_all = 0x0001; //currently all cluster implementations are according to ZCL6, which has revision #1. In the future it is possible that different clusters will have different revisions, so they will have to use separate attribute variables.

// Basic Cluster
const uint8 zclZigmo_HWRevision = ZIGMO_HWVERSION;
const uint8 zclZigmo_ZCLVersion = ZIGMO_ZCLVERSION;
const uint8 zclZigmo_ManufacturerName[] = { 16, 'T','e','x','a','s','I','n','s','t','r','u','m','e','n','t','s' };
const uint8 zclZigmo_ModelId[] = { 16, 'T','I','0','0','0','2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
const uint8 zclZigmo_DateCode[] = { 16, '2','0','0','6','0','8','3','1',' ',' ',' ',' ',' ',' ',' ',' ' };
const uint8 zclZigmo_PowerSource = POWER_SOURCE_MAINS_1_PHASE;

uint8 zclZigmo_LocationDescription[17];
uint8 zclZigmo_PhysicalEnvironment;
uint8 zclZigmo_DeviceEnable = DEVICE_ENABLED;

// Identify Cluster
uint16 zclZigmo_IdentifyTime = 0;



/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */
 
  // NOTE: The attributes listed in the AttrRec must be in ascending order 
  // per cluster to allow right function of the Foundation discovery commands
 
CONST zclAttrRec_t zclZigmo_Attrs[] =
{
  // *** General Basic Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_ZCL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclZigmo_ZCLVersion
    }
  },  
  {
    ZCL_CLUSTER_ID_GEN_BASIC,             // Cluster IDs - defined in the foundation (ie. zcl.h)
    {  // Attribute record
      ATTRID_BASIC_HW_VERSION,            // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
      ZCL_DATATYPE_UINT8,                 // Data Type - found in zcl.h
      ACCESS_CONTROL_READ,                // Variable access control - found in zcl.h
      (void *)&zclZigmo_HWRevision  // Pointer to attribute variable
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclZigmo_ManufacturerName
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MODEL_ID,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclZigmo_ModelId
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DATE_CODE,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclZigmo_DateCode
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_POWER_SOURCE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ,
      (void *)&zclZigmo_PowerSource
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_LOCATION_DESC,
      ZCL_DATATYPE_CHAR_STR,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)zclZigmo_LocationDescription
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_PHYSICAL_ENV,
      ZCL_DATATYPE_ENUM8,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclZigmo_PhysicalEnvironment
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DEVICE_ENABLED,
      ZCL_DATATYPE_BOOLEAN,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclZigmo_DeviceEnable
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclZigmo_clusterRevision_all
    }
  },  

  // *** Identify Cluster Attribute ***
  {
    ZCL_CLUSTER_ID_GEN_IDENTIFY,
    { // Attribute record
      ATTRID_IDENTIFY_TIME,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclZigmo_IdentifyTime
    }
  },  
  {
    ZCL_CLUSTER_ID_GEN_IDENTIFY,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ | ACCESS_GLOBAL,
      (void *)&zclZigmo_clusterRevision_all
    }
  },

  // *** Groups Cluster *** //
  {
    ZCL_CLUSTER_ID_GEN_GROUPS,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ | ACCESS_CLIENT,
      (void *)&zclZigmo_clusterRevision_all
    }
  },
};
uint8 CONST zclZigmo_NumAttributes = ( sizeof(zclZigmo_Attrs) / sizeof(zclZigmo_Attrs[0]) );



/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t zclZigmo_InClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_BASIC,
  ZCL_CLUSTER_ID_GEN_IDENTIFY,
};

#define ZCLZIGMO_MAX_INCLUSTERS    ( sizeof( zclZigmo_InClusterList ) / sizeof( zclZigmo_InClusterList[0] ))

const cId_t zclZigmo_OutClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_IDENTIFY,
  ZCL_CLUSTER_ID_GEN_GROUPS,
};

#define ZCLZIGMO_MAX_OUTCLUSTERS   ( sizeof( zclZigmo_OutClusterList ) / sizeof( zclZigmo_OutClusterList[0] ))

SimpleDescriptionFormat_t zclZigmo_SimpleDesc =
{
  ZIGMO_ENDPOINT,                  //  int Endpoint;
  ZCL_HA_PROFILE_ID,                  //  uint16 AppProfId[2];
  ZCL_HA_DEVICEID_ON_OFF_LIGHT_SWITCH,//  uint16 AppDeviceId[2];
  ZIGMO_DEVICE_VERSION,            //  int   AppDevVer:4;
  ZIGMO_FLAGS,                     //  int   AppFlags:4;
  ZCLZIGMO_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)zclZigmo_InClusterList, //  byte *pAppInClusterList;
  ZCLZIGMO_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)zclZigmo_OutClusterList //  byte *pAppInClusterList;
};


// Sensor array

ZigmoSensorEndpoint zigmo_endpoints[ZIGMO_NUM_SENSORS];

CONST zclAttrRec_t zigmo_sensor_attrs[][ZIGMO_NUM_SENSOR_ZCL_ATTR] = {
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[0].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[1].measuredValue),
/*  
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[2].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[3].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[4].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[5].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[6].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&zigmo_endpoints[7].measuredValue),
*/
};

// Number of attrs in zigmo_sensor_attrs must == NUM_SENSORS
STATIC_ASSERT(
              (sizeof(zigmo_sensor_attrs) / sizeof(zclAttrRec_t) / ZIGMO_NUM_SENSOR_ZCL_ATTR) == ZIGMO_NUM_SENSORS, 
              num_attr_mismatch_w_sensors);

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

void zclZigmo_InitSensorEndpoint(ZigmoSensorEndpoint* ep,
                                 uint8 endpointId) 
{ 
  zigmo_init_endpoint(ep,
                      endpointId,
                      ZIGMO_DEVICE_VERSION,
                      zigmo_sensor_attrs[endpointId - ZIGMO_FIRST_SENSOR_ENDPOINT]);
 
}


/*********************************************************************
 * @fn      zclSampleLight_ResetAttributesToDefaultValues
 *
 * @brief   Reset all writable attributes to their default values.
 *
 * @param   none
 *
 * @return  none
 */
void zclZigmo_ResetAttributesToDefaultValues(void)
{
  int i;
  
  zclZigmo_LocationDescription[0] = 16;
  for (i = 1; i <= 16; i++)
  {
    zclZigmo_LocationDescription[i] = ' ';
  }
  
  zclZigmo_PhysicalEnvironment = DEFAULT_PHYSICAL_ENVIRONMENT;
  zclZigmo_DeviceEnable = DEFAULT_DEVICE_ENABLE_STATE;
  
  zclZigmo_IdentifyTime = DEFAULT_IDENTIFY_TIME;
    
  for (i = 0; i < ZIGMO_NUM_SENSORS; i++) 
  {
    zigmo_endpoints[i].measuredValue = -1;
  }
}

/****************************************************************************
****************************************************************************/


