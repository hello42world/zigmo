#ifndef ZIGMO_MOISTURE_SENSOR_H
#define ZIGMO_MOISTURE_SENSOR_H

#include "util.h"

#include "zcl.h"
#include "zcl_ms.h"
#include "zcl_general.h"


// Defines

#define ZIGMO_NUM_SENSORS 2


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

extern uint8 zigmo_moisture_sensor_read(uint8 sensor_num);


#endif