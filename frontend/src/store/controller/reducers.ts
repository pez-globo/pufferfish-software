import { combineReducers } from 'redux';
import {
  AlarmLimits,
  BatteryPower,
  CycleMeasurements,
  Parameters,
  ScreenStatus,
  SensorMeasurements,
} from './proto/mcu_pb';
import { messageReducer, nextLogEventsReducer, rotaryEncoderReducer } from './reducers/backend';
import {
  alarmLimitsReducer,
  alarmLimitsRequestStandbyReducer,
  alarmMuteRequestReducer,
  expectedLogEventReducer,
  frontendDisplaySettingReducer,
  heartbeatBackendReducer,
  parametersRequestReducer,
  parametersRequestStanbyReducer,
  systemSettingRequestReducer,
} from './reducers/components';
import {
  activeLogEventsReducer,
  pvHistoryReducer,
  waveformHistoryReducer,
  sensorMeasurementSmoothingReducer,
} from './reducers/derived';
import { MessageType } from './types';

export const controllerReducer = combineReducers({
  // Message states from mcu_pb
  alarmLimits: messageReducer<AlarmLimits>(MessageType.AlarmLimits, AlarmLimits),
  alarmLimitsRequest: alarmLimitsReducer,
  alarmLimitsRequestStandby: alarmLimitsRequestStandbyReducer,
  alarmMuteRequest: alarmMuteRequestReducer,
  systemSettingRequest: systemSettingRequestReducer,
  frontendDisplaySetting: frontendDisplaySettingReducer,
  expectedLogEvent: expectedLogEventReducer,
  nextLogEvents: nextLogEventsReducer,
  heartbeatBackend: heartbeatBackendReducer,
  batteryPower: messageReducer<BatteryPower>(MessageType.BatteryPower, BatteryPower),
  screenStatus: messageReducer<ScreenStatus>(MessageType.ScreenStatus, ScreenStatus),
  activeLogEvents: activeLogEventsReducer,
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
      0.5,
      1.0,
      200,
      500,
      (sensorMeasurements) => sensorMeasurements.spo2,
    ),
    hr: sensorMeasurementSmoothingReducer(
      0.4,
      5.0,
      100,
      2000,
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
