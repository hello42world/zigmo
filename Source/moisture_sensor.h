#ifndef ZIGMO_MOISTURE_SENSOR_H
#define ZIGMO_MOISTURE_SENSOR_H

#include "util.h"

#include "zcl.h"
#include "zcl_ms.h"
#include "zcl_general.h"


// Defines

#define ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(measured_val_ptr) \
{ \
  { \
    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, \
    { \
      ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE, \
      ZCL_DATATYPE_INT16, \
      ACCESS_CONTROL_READ | ACCESS_REPORTABLE, \
      (void *)measured_val_ptr \
    } \
  }, \
  { \
    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, \
    { \
      ATTRID_MS_RELATIVE_HUMIDITY_MIN_MEASURED_VALUE, \
      ZCL_DATATYPE_INT16, \
      ACCESS_CONTROL_READ, \
      (void *)&zclZigmoHumidity_MinMeasuredValue \
    } \
  }, \
  { \
    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, \
    { \
      ATTRID_MS_RELATIVE_HUMIDITY_MAX_MEASURED_VALUE, \
      ZCL_DATATYPE_INT16, \
      ACCESS_CONTROL_READ, \
      (void *)&zclZigmoHumidity_MaxMeasuredValue \
    } \
  }, \
  { \
    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, \
    { \
      ATTRID_CLUSTER_REVISION, \
      ZCL_DATATYPE_UINT16, \
      ACCESS_CONTROL_READ, \
      (void *)&zclZigmo_clusterRevision_all \
    } \
  } \
}

#define ZIGMO_NUM_SENSOR_ZCL_ATTR 4

// Typedefs

typedef struct {
  const __code zclAttrRec_t* pAttrs;
  SimpleDescriptionFormat_t simpleDesc;

  int16 measuredValue;
} ZigmoSensorEndpoint;


// Vars

extern const int16 zclZigmoHumidity_MinMeasuredValue;
extern const int16 zclZigmoHumidity_MaxMeasuredValue;

// Functions.


ZStatus_t zigmo_moisture_sensor_init_endpoint(
  ZigmoSensorEndpoint* ep,
  uint8 endpoint_id,
  const zclAttrRec_t __code* attrs,
  zclGeneral_AppCallbacks_t* cmd_callbacks);


extern uint8 zigmo_moisture_sensor_read(uint8 sensor_num);

#endif