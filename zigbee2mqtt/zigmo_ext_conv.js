const {deviceEndpoints, battery, humidity, reconfigureReportingsOnDeviceAnnounce} = require('zigbee-herdsman-converters/lib/modernExtend');

const definition = {
    zigbeeModel: ['TI0002          '],
    model: 'TI0002          ',
    vendor: 'ZIGMO Sensor (4)',
    description: 'Automatically generated definition',
    extend: [
        deviceEndpoints({"endpoints":{"8":8,"9":9,"10":10,"11":11,"12":12}}),
        battery(),
        humidity({"endpointNames":["9","10","11","12"]}),
        reconfigureReportingsOnDeviceAnnounce(),
    ],
    meta: {"multiEndpoint":true},
};

module.exports = definition;
