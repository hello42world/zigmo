#ifndef ZIGMO_MOISTURE_SENSOR_H
#define ZIGMO_MOISTURE_SENSOR_H

#include "util.h"

#include "zcl.h"
#include "zcl_ms.h"
#include "zcl_general.h"


// Defines
#define ZIGMO_SENSOR_PWR_PIN   P1_1
#define ZIGMO_SENSOR_SEL_A_PIN P1_2
#define ZIGMO_SENSOR_SEL_B_PIN P1_3

#define ZIGMO_NUM_SENSORS 4


// Typedefs

typedef struct {
  const __code zclAttrRec_t* pAttrs;
  SimpleDescriptionFormat_t simpleDesc;
  int16 measuredValue;
} ZigmoSensorEndpoint;


// Vars

extern const int16 zclZigmoHumidity_MinMeasuredValue;
extern const int16 zclZigmoHumidity_MaxMeasuredValue;
//extern CONST zclAttrRec_t g_zigmo_sensor_attrs[][ZIGMO_NUM_SENSOR_ZCL_ATTR];

// Functions.

ZStatus_t zigmo_moisture_sensor_init_sensors(
  uint8 first_endpoint_num,
  zclGeneral_AppCallbacks_t* cmd_callbacks);

extern void zigmo_moisture_sensors_refresh(uint8 first_endpoint_num);


#endif