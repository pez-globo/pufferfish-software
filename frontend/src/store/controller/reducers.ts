import { combineReducers } from 'redux';
import {
  ActiveLogEvents,
  Alarms,
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
  parametersRequestReducer,
  parametersRequestStanbyReducer,
  systemSettingRequestReducer,
} from './reducers/components';
import { pvHistoryReducer, waveformHistoryReducer } from './reducers/derived';
import { MessageType } from './types';

export const controllerReducer = combineReducers({
  // Message states from mcu_pb
  alarms: messageReducer<Alarms>(MessageType.Alarms, Alarms),
  alarmLimitsRequest: alarmLimitsReducer,
  alarmLimitsRequestStandby: alarmLimitsRequestStandbyReducer,
  alarmMuteRequest: alarmMuteRequestReducer,
  parametersRequestStandby: parametersRequestStanbyReducer,
  systemSettingRequest: systemSettingRequestReducer,
  frontendDisplaySetting: frontendDisplaySettingReducer,
  expectedLogEvent: expectedLogEventReducer,
  nextLogEvents: nextLogEventsReducer,
  batteryPower: messageReducer<BatteryPower>(MessageType.BatteryPower, BatteryPower),
  screenStatus: messageReducer<ScreenStatus>(MessageType.ScreenStatus, ScreenStatus),
  activeLogEvents: messageReducer<ActiveLogEvents>(MessageType.ActiveLogEvents, ActiveLogEvents),
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

  // Message states from frontend_pb
  rotaryEncoder: rotaryEncoderReducer,

  // Derived states
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
