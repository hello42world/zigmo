#ifndef ZIGMO_MOISTURE_SENSOR_H
#define ZIGMO_MOISTURE_SENSOR_H

#include "zcl.h"
#include "zcl_ms.h"

// Hi C

#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]

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
  CONST zclAttrRec_t* pAttrs;
  SimpleDescriptionFormat_t simpleDesc;
  endPointDesc_t endpoint;
  
  int16 measuredValue;
} zigmoSensorEndpoint;


// Vars

extern const int16 zclZigmoHumidity_MinMeasuredValue; 
extern const int16 zclZigmoHumidity_MaxMeasuredValue;

#endif