#include "moisture_sensor.h"

#include "zcl_ha.h"
#include "bdb.h"
#include "bdb_interface.h"

const int16 zclZigmoHumidity_MinMeasuredValue = 0; 
const int16 zclZigmoHumidity_MaxMeasuredValue = 10000;


static const cId_t _InClusterList[] =
{
  ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY
};

void zigmo_init_endpoint(ZigmoSensorEndpoint* ep,
                         uint8 endpoint_id,
                         uint8 device_version,
                         const zclAttrRec_t __code* attrs) 
{
  // Init attrs
  ep->pAttrs = attrs;
  
  // Init simple descriptor
  ep->simpleDesc.EndPoint = endpoint_id;
  ep->simpleDesc.AppProfId = ZCL_HA_PROFILE_ID;
  ep->simpleDesc.AppDeviceId = ZCL_HA_DEVICEID_SIMPLE_SENSOR;
  ep->simpleDesc.AppDevVer = device_version;
  ep->simpleDesc.Reserved = 0;
  ep->simpleDesc.AppNumInClusters = 1;
  ep->simpleDesc.pAppInClusterList = (cId_t*) _InClusterList;
  ep->simpleDesc.AppNumOutClusters = 0;
  ep->simpleDesc.pAppOutClusterList = NULL;

    
  // Init measured value
  ep->measuredValue = -1;
}

#if BDBREPORTING_MAX_ANALOG_ATTR_SIZE == 8
  uint8 reportableChange[] = {0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
#endif
#if BDBREPORTING_MAX_ANALOG_ATTR_SIZE == 4
  uint8 reportableChange[] = {0x64, 0x00, 0x00, 0x00}; 
#endif 
#if BDBREPORTING_MAX_ANALOG_ATTR_SIZE == 2
  uint8 reportableChange[] = {0x64, 0x00};
#endif 

ZStatus_t zigmo_register_endpoint(ZigmoSensorEndpoint* ep,
                             uint8 endpoint_id,
                             zclGeneral_AppCallbacks_t* cmd_callbacks) 
{
  ZStatus_t s;

  bdb_RegisterSimpleDescriptor(&ep->simpleDesc);
  
  s = zclGeneral_RegisterCmdCallbacks(endpoint_id, cmd_callbacks );
  if (s != ZSuccess) {
    return s;
  }
  
  s = zcl_registerAttrList(endpoint_id, 
                           ZIGMO_NUM_SENSOR_ZCL_ATTR, 
                           ep->pAttrs );
  if (s != ZSuccess) {
    return s;
  }
                             
  s = bdb_RepAddAttrCfgRecordDefaultToList(
                                   endpoint_id, 
                                   ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, 
                                   ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE, 
                                   0, 10, reportableChange);
  
  return s;   
}