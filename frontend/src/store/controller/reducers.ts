import { combineReducers } from 'redux';
import {
  AlarmLimits,
  Battery,
  CycleMeasurements,
  Parameters,
  ScreenStatus,
  SensorMeasurements,
} from './proto/mcu_pb';
import { messageReducer, eventLogReducer, rotaryEncoderReducer } from './reducers/backend';
import {
  alarmLimitsReducer,
  alarmLimitsRequestStandbyReducer,
  alarmMuteRequestReducer,
  frontendDisplaySettingReducer,
  heartbeatBackendReducer,
  parametersRequestReducer,
  parametersRequestStanbyReducer,
  systemSettingRequestReducer,
} from './reducers/components';
import {
  pvHistoryReducer,
  waveformHistoryReducer,
  sensorMeasurementSmoothingReducer,
} from './reducers/derived';
import { MessageType } from './types';

export const controllerReducer = combineReducers({
  // Message states from mcu_pb
  alarmLimits: messageReducer<AlarmLimits>(MessageType.AlarmLimits, AlarmLimits, {
    // Needed so that values aren't undefined when dashboard is loaded before redirecting to quickstart
    spo2: { lower: 90, upper: 100 },
    hr: { lower: 60, upper: 100 },
    fio2: { lower: 78, upper: 82 },
    flow: { lower: 28, upper: 32 },
  }),
  alarmLimitsRequest: alarmLimitsReducer,
  alarmLimitsRequestStandby: alarmLimitsRequestStandbyReducer,
  alarmMuteRequest: alarmMuteRequestReducer,
  systemSettingRequest: systemSettingRequestReducer,
  frontendDisplaySetting: frontendDisplaySettingReducer,
  eventLog: eventLogReducer,
  heartbeatBackend: heartbeatBackendReducer,
  batteryPower: messageReducer<Battery>(MessageType.Battery, Battery),
  screenStatus: messageReducer<ScreenStatus>(MessageType.ScreenStatus, ScreenStatus),
  sensorMeasurements: messageReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    SensorMeasurements,
  ),
  cycleMeasurements: messageReducer<CycleMeasurements>(
    MessageType.CycleMeasurements,
    CycleMeasurements,
  ),
  parameters: messageReducer<Parameters>(MessageType.Parameters, Parameters),
  parametersRequest: parametersRequestReducer,
  parametersRequestStandby: parametersRequestStanbyReducer,

  // Message states from frontend_pb
  rotaryEncoder: rotaryEncoderReducer,

  // Derived states
  smoothedMeasurements: combineReducers({
    fio2: sensorMeasurementSmoothingReducer(
      0.5,
      0.1,
      500,
      200,
      (sensorMeasurements) => sensorMeasurements.fio2,
    ),
    flow: sensorMeasurementSmoothingReducer(
      0.5,
      0.5,
      500,
      200,
      (sensorMeasurements) => sensorMeasurements.flow,
    ),
    spo2: sensorMeasurementSmoothingReducer(
      1,
      1.0,
      200,
      1000,
      (sensorMeasurements) => sensorMeasurements.spo2,
    ),
    hr: sensorMeasurementSmoothingReducer(
      1,
      1.0,
      200,
      1000,
      (sensorMeasurements) => sensorMeasurements.hr,
    ),
  }),
  waveformHistoryPaw: waveformHistoryReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.paw,
    60,
    0,
  ),
  waveformHistoryFlow: waveformHistoryReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.flow,
    60,
    20,
  ),
  waveformHistoryVolume: waveformHistoryReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.volume,
    60,
    40,
  ),
  pvHistory: pvHistoryReducer,
});

export default controllerReducer;
