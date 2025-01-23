#include "moisture_sensor.h"

#include "hal2.h"

#include "OnBoard.h"
#include "zcl_ha.h"
#include "bdb.h"
#include "bdb_interface.h"
#include "hal_adc.h"
#include "dbg.h"

#define ZIGMO_NUM_SENSOR_ZCL_ATTR 4

#define ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(measured_val_ptr) \
{ \
  { \
    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, \
    { \
      ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE, \
      ZCL_DATATYPE_UINT16, \
      ACCESS_CONTROL_READ | ACCESS_REPORTABLE, \
      (void *)measured_val_ptr \
    } \
  }, \
  { \
    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, \
    { \
      ATTRID_MS_RELATIVE_HUMIDITY_MIN_MEASURED_VALUE, \
      ZCL_DATATYPE_UINT16, \
      ACCESS_CONTROL_READ, \
      (void *)&zclZigmoHumidity_MinMeasuredValue \
    } \
  }, \
  { \
    ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY, \
    { \
      ATTRID_MS_RELATIVE_HUMIDITY_MAX_MEASURED_VALUE, \
      ZCL_DATATYPE_UINT16, \
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
      (void *)&zigmo_cluster_revision \
    } \
  } \
}

const int16 zclZigmoHumidity_MinMeasuredValue = 0;
const int16 zclZigmoHumidity_MaxMeasuredValue = 10000;
const uint16 zigmo_cluster_revision = 0x0001;

static const cId_t _InClusterList[] =
{
  ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY
};


// Sensor array

ZigmoSensorEndpoint g_zigmo_endpoints[ZIGMO_NUM_SENSORS];

CONST zclAttrRec_t g_zigmo_sensor_attrs[][ZIGMO_NUM_SENSOR_ZCL_ATTR] = {
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[0].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[1].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[2].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[3].measuredValue),
/*
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[4].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[5].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[6].measuredValue),
  ZIGMO_DECLARE_SENSOR_ZCL_ATTRS(&g_zigmo_endpoints[7].measuredValue),
*/
};

// Number of attrs in zigmo_sensor_attrs must == NUM_SENSORS
STATIC_ASSERT(
              (sizeof(g_zigmo_sensor_attrs) / sizeof(zclAttrRec_t) / ZIGMO_NUM_SENSOR_ZCL_ATTR) == ZIGMO_NUM_SENSORS,
              num_attr_mismatch_w_sensors);

STATIC_ASSERT(BDBREPORTING_MAX_ANALOG_ATTR_SIZE == 4, Wrong_bdb_attr_size);
// 500 or 5% is the min reportable change.
uint8 moistureReportableChange[] = {0xF4, 0x01, 0x00, 0x00};

static ZStatus_t zigmo_sensor_register_endpoint(
  ZigmoSensorEndpoint* ep,
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
   0,   // No min report interval.
   600, // Max inteval is every 10 min.
   moistureReportableChange);
  return s;
}

static ZStatus_t zigmo_moisture_sensor_init_endpoint(
  ZigmoSensorEndpoint* ep,
  uint8 endpoint_id,
  const zclAttrRec_t __code* attrs,
  zclGeneral_AppCallbacks_t* cmd_callbacks)
{
  // Init attrs
  ep->pAttrs = attrs;

  // Init simple descriptor
  ep->simpleDesc.EndPoint = endpoint_id;
  ep->simpleDesc.AppProfId = ZCL_HA_PROFILE_ID;
  ep->simpleDesc.AppDeviceId = ZCL_HA_DEVICEID_SIMPLE_SENSOR;
  ep->simpleDesc.AppDevVer = 1;
  ep->simpleDesc.Reserved = 0;
  ep->simpleDesc.AppNumInClusters = 1;
  ep->simpleDesc.pAppInClusterList = (cId_t*) _InClusterList;
  ep->simpleDesc.AppNumOutClusters = 0;
  ep->simpleDesc.pAppOutClusterList = NULL;


  // Init measured value
  ep->measuredValue = -1;

  return zigmo_sensor_register_endpoint(ep, endpoint_id, cmd_callbacks);
}

ZStatus_t zigmo_moisture_sensor_init_sensors(
  uint8 first_endpoint_num,
  zclGeneral_AppCallbacks_t* cmd_callbacks)
{
  //
  // Init pins
  //

  // Sensor power pin
  // Set P1_1 to GPIO
  P1SEL &= ~(1 << 1);
  // Set P1_1 direction to Output.
  P1DIR |= (1 << 1);

  // Channel select pin 0
  // Set P1_2 to GPIO
  P1SEL &= ~(1 << 2);
  // Set P1_1 direction to Output.
  P1DIR |= (1 << 2);

  // Channel select pin 1
  // Set P1_3 to GPIO
  P1SEL &= ~(1 << 3);
  // Set P1_3 direction to Output.
  P1DIR |= (1 << 3);

  // Sensor ADC pin
  // Enable ADC on P0_4
  APCFG |= (1 << 4);

  //
  // Init endpoints
  //
  for (uint8 i = 0; i < ZIGMO_NUM_SENSORS; i++)
  {
    ZStatus_t s = zigmo_moisture_sensor_init_endpoint(
      &g_zigmo_endpoints[i],
      first_endpoint_num + i,
      g_zigmo_sensor_attrs[i],
      cmd_callbacks);
    if (s != ZSuccess) {
      return s;
    }
  }

  return ZSuccess;
}

// 10 bit ADC
//#define ADC_MAX 250
//#define ADC_MIN 110

// 12 bit ADC
#define ADC_MAX 1300
#define ADC_MIN 760


static int16 zigmo_moisture_sensor_read(uint8 sensor_num)
{
  // Enable the corresponding multiplexer channel.
  ZIGMO_SENSOR_SEL_A_PIN = (sensor_num >> 0) & 0x01; // A
  ZIGMO_SENSOR_SEL_B_PIN = (sensor_num >> 1) & 0x01; // B

  zigmo_util_delay(4000);

  int32 adc;
  int32 ksave0 = 0;

  /* Keep on reading the ADC until two consecutive key decisions are the same. */
  do
  {
    ksave0 = adc;
    adc = HalAdcRead (HAL_ADC_CHN_AIN4, HAL_ADC_RESOLUTION_12);
  } while (adc != ksave0);

  if (adc < 60) {
    return 0; // No input signal.
  }

  if (adc < ADC_MIN) adc = ADC_MIN;
  if (adc > ADC_MAX) adc = ADC_MAX;

  adc = (ADC_MAX - adc) * 100 * 100  / (ADC_MAX - ADC_MIN);

  return (int16)adc;
}


void zigmo_moisture_sensors_refresh(uint8 first_endpoint_num)
{
  HalAdcSetReference(HAL_ADC_REF_125V);
  ZIGMO_SENSOR_PWR_PIN = 1; // Power on the sensor circuit.

  for (int i = 0; i < ZIGMO_NUM_SENSORS; i++)
  {
    g_zigmo_endpoints[i].measuredValue = zigmo_moisture_sensor_read(i);

    bdb_RepChangedAttrValue(
      first_endpoint_num + i,
      ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY,
      ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE);
  }
  ZIGMO_SENSOR_PWR_PIN = 0;
/*
  dprintf("%d %d %d %d",
          g_zigmo_endpoints[0].measuredValue,
          g_zigmo_endpoints[1].measuredValue,
          g_zigmo_endpoints[2].measuredValue,
          g_zigmo_endpoints[3].measuredValue);
*/
}